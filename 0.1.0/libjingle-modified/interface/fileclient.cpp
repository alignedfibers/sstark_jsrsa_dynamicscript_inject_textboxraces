#include <iomanip>
#include <time.h>

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iomanip>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#else
#include <direct.h>
#include "talk/base/win32.h"
#endif

#include "fileclient.h"
#include "jinglewrapper.h"
#include "errorhandler.h"
#include "contactsmanager.h"

#if defined(_MSC_VER) && (_MSC_VER < 1400)
// The following are necessary to properly link when compiling STL without
// /EHsc, otherwise known as C++ exceptions.
void __cdecl std::_Throw(const std::exception &) {}

std::_Prhand std::_Raise_handler = 0;
#endif

FileClient::FileClient(buzz::XmppClient *xmppclient, const buzz::Jid &send_to,
                       CJingleWrapper* jingw) :
		xmpp_client_(xmppclient),
		waiting_for_file_(send_to == buzz::JID_EMPTY),
		session_(NULL),
		sendsession_(NULL),
		sendmanifest_(NULL),
		jingw_(jingw)
{
	xmpp_client_->SignalStateChange.connect(this, &FileClient::OnStateChange);
}

FileClient::~FileClient()
{
	talk_base::Thread *thread = talk_base::ThreadManager::CurrentThread();
	delete session_;
	thread->Stop();
}

void FileClient::OnStateChange(buzz::XmppEngine::State state)
{
	switch (state)
	{

		case buzz::XmppEngine::STATE_START:
			std::cout << "Connecting..." << std::endl;
			break;

		case buzz::XmppEngine::STATE_OPENING:
			std::cout << "Logging in. " << std::endl;
			break;

		case buzz::XmppEngine::STATE_OPEN:
			std::cout << "Logged in as " << xmpp_client_->jid().Str() << std::endl;
			OnSignon();
			break;

		case buzz::XmppEngine::STATE_CLOSED:
			std::cout << "Logged out." << std::endl;
			break;
	}
}

void FileClient::OnJingleInfo(const std::string & relay_token,
                              const std::vector<std::string> &relay_addresses,
                              const std::vector<talk_base::SocketAddress> &stun_addresses)
{
	port_allocator_->SetStunHosts(stun_addresses);
	port_allocator_->SetRelayHosts(relay_addresses);
	port_allocator_->SetRelayToken(relay_token);
}


void FileClient::OnStatusUpdate(const buzz::Status &status)
{
}

void FileClient::OnMessage(talk_base::Message *m)
{
	if (m->message_id == MSG_STOP)
	{
		waiting_for_file_ = true;
		return;
	}

	if (m->message_id == MSG_SENDFILE)
	{
		if (waiting_for_file_ == false)
		{
			cb_->OnError(eCannotStartFileTransfer, ERR2STR(eCannotStartFileTransfer));
			return;
		}

		waiting_for_file_ = false;

		talk_base::TypedMessageData<std::string> *data =
		    static_cast<talk_base::TypedMessageData<std::string>*>(m->pdata);

		std::string sMsg(data->data());
		int sep = sMsg.find(' ');
		std::string sContact(sMsg.substr(0, sep));
		std::string sPath(sMsg.substr(sep+1));

		int lastSlashPos = sPath.rfind('/', sPath.size() - 2);
		std::string rootFld = sPath.substr(0, lastSlashPos);
		std::string fileName = sPath.substr(lastSlashPos + 1);

		bool found = false;
		buzz::Jid found_jid;
		buzz::Jid sendto_jid = buzz::Jid(sContact);
		RosterMap::const_iterator iter = jingw_->m_ContactsManager->getRoster().begin();

		while (iter != jingw_->m_ContactsManager->getRoster().end())
		{
			if (iter->second.jid.BareEquals(sendto_jid) && iter->second.status.fileshare_capability())
			{
				found = true;
				found_jid = iter->second.jid;
				break;
			}

			++iter;
		}

		if (found)
		{
			sendsession_ = file_share_session_client_->CreateFileShareSession();

			sendsession_->SetLocalFolder(rootFld);

			sendmanifest_ = new cricket::FileShareManifest();

			size_t size = 0;
			if (talk_base::Filesystem::IsFolder(fileName))
			{
				size = get_dir_size(fileName.c_str());
				sendmanifest_->AddFolder(fileName, size);
			}
			else
			{
				talk_base::Filesystem::GetFileSize(rootFld + "/" + fileName, &size);
				sendmanifest_->AddFile(fileName, size);
			}

			std::cout << "Found the contact: " << found_jid.Str() << std::endl;
			sendsession_->Share(found_jid.Str(), const_cast<cricket::FileShareManifest*>(sendmanifest_));
		}
		else
		{
			if (cb_)
			{
				cb_->OnError(eCannotFindContact, ERR2STR(eCannotFindContact));
				cb_->OnFileTransferProgress(-1, -1, root_dir_);
			}
		}

		waiting_for_file_ = true;
	}


	if (m->message_id == MSG_CANCELTRANSFER)
	{
		if (session_)
			session_->Cancel();
	}
}

void FileClient::OnSessionState(cricket::FileShareState state)
{
	talk_base::Thread *thread = talk_base::ThreadManager::CurrentThread();
	std::stringstream manifest_description;

	switch (state)
	{

		case cricket::FS_OFFER:
			// The offer has been made; print a summary of it and, if it's an incoming transfer, accept it
			if (session_->manifest()->size() == 1)
				manifest_description <<  session_->manifest()->item(0).name;
			else if (session_->manifest()->GetFileCount() && session_->manifest()->GetFolderCount())
				manifest_description <<  session_->manifest()->GetFileCount() << " files and " <<
				session_->manifest()->GetFolderCount() << " directories";
			else if (session_->manifest()->GetFileCount() > 0)
				manifest_description <<  session_->manifest()->GetFileCount() << " files";
			else
				manifest_description <<  session_->manifest()->GetFolderCount() << " directories";

			size_t filesize;

			if (!session_->GetTotalSize(filesize))
			{
				manifest_description << " (Unknown size)";
				filesize = 1;
			}
			else
			{
				manifest_description << " (" << filesize << " Bytes)";
			}

			if (session_->is_sender())
			{
				std::cout << "Offering " << manifest_description.str() << std::endl;
			}
			else if (waiting_for_file_)
			{
				std::cout << "Receiving " << manifest_description.str() << " from " << session_->jid().BareJid().Str() << std::endl;

				cb_->OnFileReceiveStart(session_->jid().BareJid().Str(), filesize, session_->manifest()->item(0).name, root_dir_);

				if (root_dir_ == "")
				{
					session_->Decline();
					return;
				}

				session_->SetLocalFolder(root_dir_);

				session_->Accept();
				waiting_for_file_ = false;

				// If this were a graphical client, we might want to go through the manifest, look for images,
				// and request previews. There are two ways to go about this:
				//
				// If we want to display the preview in a web browser (like the embedded IE control in Google Talk), we could call
				// GetImagePreviewUrl on the session, with the image's index in the manifest, the size, and a pointer to the URL.
				// This will cause the session to listen for HTTP requests on localhost, and set url to a localhost URL that any
				// web browser can use to get the image preview:
				//
				//      std::string url;
				//      session_->GetImagePreviewUrl(0, 100, 100, &url);
				//      url = std::string("firefox \"") + url + "\"";
				//      system(url.c_str());
				//
				// Alternately, you could use libjingle's own HTTP code with the FileShareSession's SocketPool interface to
				// write the image preview directly into a StreamInterface:
				//
				//	talk_base::HttpClient *client = new talk_base::HttpClient("pcp", session_);
				//	std::string path;
				//	session_->GetItemNetworkPath(0,1,&path);
				//
				//	client->request().verb = talk_base::HV_GET;
				//	client->request().path = path + "?width=100&height=100";
				//	talk_base::FileStream *file = new talk_base::FileStream;
				//	file->Open("/home/username/foo.jpg", "wb");
				//	client->response().document.reset(file);
				//	client->start();
			}

			break;

		case cricket::FS_TRANSFER:
			session_->GetTotalSize(filesize);
			cb_->OnFileTransferProgress(filesize, 0, "");
			std::cout << "File transfer started." << std::endl;
			break;

		case cricket::FS_COMPLETE:
			session_->GetTotalSize(filesize);
			if (sendsession_)
				cb_->OnFileTransferProgress(filesize, filesize, "");
			else
				cb_->OnFileTransferProgress(filesize, filesize, root_dir_);

			delete sendsession_;
			sendsession_ = NULL;
			thread->Post(this, MSG_STOP);
			std::cout << std::endl << "File transfer completed. rootdir: " << root_dir_.c_str() << std::endl;
			break;

		case cricket::FS_LOCAL_CANCEL:
		case cricket::FS_REMOTE_CANCEL:
			cb_->OnFileTransferProgress(-1, -1, "");
			delete sendsession_;
			sendsession_ = NULL;
			std::cout << std::endl << "File transfer cancelled." << std::endl;
			thread->Post(this, MSG_STOP);
			break;

		case cricket::FS_FAILURE:
			cb_->OnFileTransferProgress(-1, -1, "");
			delete sendsession_;
			sendsession_ = NULL;
			std::cout << std::endl << "File transfer failed." << std::endl;
			thread->Post(this, MSG_STOP);
			break;
	}
}

void FileClient::OnUpdateProgress(cricket::FileShareSession *sess)
{
	// Progress has occured on the transfer; update the UI

	size_t totalsize, progress;
	std::string itemname;
	unsigned int width = 79;

	sess->GetTotalSize(totalsize);
	sess->GetProgress(progress);
	float percent = (float)progress / totalsize;

#ifndef WIN32

	struct winsize ws;

	if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0))
		width = ws.ws_col;

#endif

	if (sess->GetTotalSize(totalsize) && sess->GetProgress(progress) && sess->GetCurrentItemName(&itemname))
	{
		if (progress < totalsize)
			cb_->OnFileTransferProgress(totalsize, progress, "");
	}
}

void FileClient::OnResampleImage(std::string path, int width, int height, talk_base::HttpTransaction *trans)
{
	std::cout << std::endl << "OnResampleImage()" << std::endl;

	// The other side has requested an image preview. This is an asynchronous request. We should resize
	// the image to the requested size,and send that to ResampleComplete(). For simplicity, here, we
	// send back the original sized image. Note that because we don't recognize images in our manifest
	// this will never be called in pcp

	// Even if you don't resize images, you should implement this method and connect to the
	// SignalResampleImage signal, just to return an error.

	//talk_base::FileStream *s = new talk_base::FileStream();
	//if (s->Open(path.c_str(), "rb"))
	//  session_->ResampleComplete(s, trans, true);
	//else {
	//  delete s;
	session_->ResampleComplete(NULL, trans, false);
	//}
}

void FileClient::OnFileShareSessionCreate(cricket::FileShareSession *sess)
{
	//std::cout << std::endl << "OnFileShareSessionCreate()" << std::endl;

	session_ = sess;
	sess->SignalState.connect(this, &FileClient::OnSessionState);
	sess->SignalNextFile.connect(this, &FileClient::OnUpdateProgress);
	sess->SignalUpdateProgress.connect(this, &FileClient::OnUpdateProgress);
	sess->SignalResampleImage.connect(this, &FileClient::OnResampleImage);
	sess->SetLocalFolder(root_dir_);
}

void FileClient::OnSignon()
{
	//std::cout << std::endl << "OnSignon()" << std::endl;

	std::string client_unique = xmpp_client_->jid().Str();
	cricket::InitRandom(client_unique.c_str(), client_unique.size());

	buzz::PresencePushTask *presence_push_ = new buzz::PresencePushTask(xmpp_client_);
	presence_push_->SignalStatusUpdate.connect(this, &FileClient::OnStatusUpdate);
	presence_push_->Start();

	buzz::Status my_status(jingw_->m_ContactsManager->myStatus_);
	my_status.set_fileshare_capability(true);
    my_status.set_phone_capability(false);

	buzz::PresenceOutTask* presence_out_ =
	    new buzz::PresenceOutTask(xmpp_client_);
	presence_out_->Send(my_status);
	presence_out_->Start();

	port_allocator_.reset(new cricket::HttpPortAllocator(&network_manager_, "pcp"));

	session_manager_.reset(new cricket::SessionManager(port_allocator_.get(), NULL));

	cricket::SessionManagerTask * session_manager_task = new cricket::SessionManagerTask(xmpp_client_, session_manager_.get());
	session_manager_task->EnableOutgoingMessages();
	session_manager_task->Start();

	buzz::JingleInfoTask *jingle_info_task = new buzz::JingleInfoTask(xmpp_client_);
	jingle_info_task->RefreshJingleInfoNow();
	jingle_info_task->SignalJingleInfo.connect(this, &FileClient::OnJingleInfo);
	jingle_info_task->Start();

	file_share_session_client_.reset(new cricket::FileShareSessionClient(session_manager_.get(), xmpp_client_->jid(), "pcp"));
	file_share_session_client_->SignalFileShareSessionCreate.connect(this, &FileClient::OnFileShareSessionCreate);
	session_manager_->AddClient(NS_GOOGLE_SHARE, file_share_session_client_.get());
}


unsigned int FileClient::get_dir_size(const char *directory)
{
	unsigned int total = 0;
	talk_base::DirectoryIterator iter;
	talk_base::Pathname path;
	path.AppendFolder(directory);
	iter.Iterate(path.pathname());

	while (iter.Next())
	{
		if (iter.Name() == "." || iter.Name() == "..")
			continue;

		if (iter.IsDirectory())
		{
			path.AppendPathname(iter.Name());
			total += get_dir_size(path.pathname().c_str());
		}
		else
			total += iter.FileSize();
	}

	return total;
}
