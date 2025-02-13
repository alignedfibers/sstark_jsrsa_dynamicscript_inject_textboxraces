#ifndef __fileclient__
#define __fileclient__


#include "talk/base/fileutils.h"
#include "talk/base/pathutils.h"
#include "talk/base/helpers.h"
#include "talk/base/httpclient.h"
#include "talk/base/logging.h"
#include "talk/base/physicalsocketserver.h"
#include "talk/base/ssladapter.h"
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/login/xmppthread.h"
#include "talk/login/xmppauth.h"
#include "talk/p2p/client/httpportallocator.h"
#include "talk/p2p/client/sessionmanagertask.h"
#include "talk/session/fileshare/fileshare.h"
#include "talk/login/presencepushtask.h"
#include "talk/login/presenceouttask.h"
#include "talk/login/jingleinfotask.h"

class CJingleCallback;

class CJingleWrapper;

class FileClient : public sigslot::has_slots<>, public talk_base::MessageHandler
{

public:
	FileClient(buzz::XmppClient *xmppclient, const buzz::Jid &send_to,
	           CJingleWrapper* jingw);
	virtual ~FileClient();

	void SetCallBack(CJingleCallback* cb)
	{
		cb_ = cb;
	}

	enum
	{
		MSG_STOP,
		MSG_SENDFILE,
		MSG_CANCELTRANSFER
	};

private:
	void OnStateChange(buzz::XmppEngine::State state);

	void OnJingleInfo(const std::string & relay_token,
	                  const std::vector<std::string> &relay_addresses,
	                  const std::vector<talk_base::SocketAddress> &stun_addresses);

	void OnStatusUpdate(const buzz::Status &status);
	void OnMessage(talk_base::Message *m);

	void OnSessionState(cricket::FileShareState state);

	void OnUpdateProgress(cricket::FileShareSession *sess);
	void OnResampleImage(std::string path, int width, int height, talk_base::HttpTransaction *trans);

	void OnFileShareSessionCreate(cricket::FileShareSession *sess);

	void OnSignon();

	static unsigned int get_dir_size(const char *directory);

	talk_base::NetworkManager network_manager_;
	talk_base::scoped_ptr<cricket::HttpPortAllocator> port_allocator_;
	talk_base::scoped_ptr<cricket::SessionManager> session_manager_;
	talk_base::scoped_ptr<cricket::FileShareSessionClient> file_share_session_client_;
	buzz::XmppClient *xmpp_client_;
	
	cricket::FileShareSession *session_;
	cricket::FileShareSession *sendsession_;
	cricket::FileShareManifest *sendmanifest_;
	bool waiting_for_file_;
	std::string root_dir_;
	CJingleCallback* cb_;
	CJingleWrapper* jingw_;
};


#endif // __fileclient__
