/*  Copyright 2008 Jozsef Bekes

    This file is part of gtalX.

    gtalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gtalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gtalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "jinglewrapper.h"

#include "contactsmanager.h"
#include "callclient.h"
#include "fileclient.h"
#include "errorhandler.h"
#include "globalsettings.h"

#include <time.h>
#include <iomanip>
#include "talk/base/logging.h"
#include "talk/base/physicalsocketserver.h"
#include "talk/base/ssladapter.h"
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/login/xmppthread.h"
#include "talk/login/xmppauth.h"

bool CJingleWrapper::m_callThreadInitialized = false;
bool CJingleWrapper::m_fileThreadInitialized = false;

std::string CJingleWrapper::m_gtalkServer;
int CJingleWrapper::m_gtalkPort;
std::string CJingleWrapper::m_proxyHost;
int CJingleWrapper::m_proxyPort;

CLibjingleIf* CLibjingleIf::create(CJingleCallback* cb,
								   const std::string& gtalkServer,
								   int gtalkPort,
								   const std::string& ringWav,
								   const std::string& callWav,
								   const std::string& chatDingWav,
								   bool logVerbose,
								   const std::string& proxyHost,
								   int proxyPort)
{
    return new CJingleWrapper(cb, gtalkServer, gtalkPort, ringWav,
							  callWav, chatDingWav, logVerbose,
							  proxyHost, proxyPort);
}

CJingleWrapper::CJingleWrapper(	CJingleCallback* cb,
								const std::string& gtalkServer,
								int gtalkPort,
								const std::string& ringWav,
								const std::string& callWav,
								const std::string& chatDingWav,
								bool logVerbose,
								const std::string& proxyHost,
								int proxyPort)  :
    m_ringWav(ringWav),
    m_callWav(callWav),
	m_chatDingWav(chatDingWav),
	m_logVerbose(logVerbose),
	m_cb(cb)
{
	m_gtalkServer = gtalkServer;
	m_gtalkPort = gtalkPort;

	if (m_gtalkServer.size() == 0)
		m_gtalkServer = "talk.google.com";

	if (m_gtalkPort == 0)
		m_gtalkPort = 5222;

	m_proxyHost = proxyHost;
	m_proxyPort = proxyPort;
}

CJingleWrapper::~CJingleWrapper()
{
	delete m_mainThread;
	m_cb = NULL;
}

void* CJingleWrapper::thread_func(void* p)
{
    CJingleWrapper* jw = (CJingleWrapper*)p;

    talk_base::InitializeSSL();

    XmppPump pump;
    buzz::Jid jid;
    buzz::XmppClientSettings xcs;
    talk_base::InsecureCryptStringImpl pass;

    jid = buzz::Jid(jw ->m_usn);
    pass.password() = jw ->m_pwd.c_str();

    xcs.set_user(jid.node());
    xcs.set_resource("call");
    xcs.set_host(jid.domain());
    xcs.set_use_tls(true);
	if (m_proxyHost.size() > 0)
	{
		std::cout << "Proxy host: " << m_proxyHost << std::endl;
		xcs.set_proxy_host(m_proxyHost);
	}

	//xcs.set_proxy_pass()
	if (m_proxyPort != 0)
	{
		std::cout << "Proxy port: " << m_proxyPort << std::endl;
		xcs.set_proxy_port(m_proxyPort);
	}
	//xcs.set_proxy_user()

    xcs.set_pass(talk_base::CryptString(pass));
	std::cout << "Connection info: " << m_gtalkServer.c_str() << ", " << m_gtalkPort << std::endl;
    xcs.set_server(talk_base::SocketAddress(m_gtalkServer.c_str(), m_gtalkPort));
    printf("Logging in as %s\n", jid.Str().c_str());

    talk_base::PhysicalSocketServer ss;

	jw->m_ContactsManager = new ContactsManager(pump.client(), jw->m_cb);

    jw->m_client = new CallClient(pump.client(), jw);
    jw->m_client->SetCallBack(jw->m_cb );
    jw->m_client->SetWavs(jw->m_ringWav, jw->m_callWav, jw->m_chatDingWav);
	jw->m_client->SetLog(jw->m_logVerbose);

    talk_base::Thread main_thread(&ss);
    jw->m_mainThread = &main_thread;
    talk_base::ThreadManager::SetCurrent(&main_thread);

    pump.DoLogin(xcs, new XmppSocket(true), NULL);

	m_callThreadInitialized = true;
    main_thread.Run();
    m_callThreadInitialized = false;
	jw->m_mainThread = NULL;
    return NULL;
}

void* CJingleWrapper::file_thread_func(void* p)
{
    CJingleWrapper* jw = (CJingleWrapper*)p;

	talk_base::InitializeSSL();
	XmppPump pump;
	buzz::Jid jid;
	buzz::XmppClientSettings xcs;
	talk_base::InsecureCryptStringImpl pass;

	jid = buzz::Jid(jw->m_usn);
	if (!jid.IsValid() || jid.node() == "") {
		printf("Invalid JID. JIDs should be in the form user@domain\n");
		return NULL;
	}
	pass.password() = jw ->m_pwd.c_str();


	xcs.set_user(jid.node());
	xcs.set_resource("pcp");
	xcs.set_host(jid.domain());
	xcs.set_use_tls(true);

	xcs.set_pass(talk_base::CryptString(pass));
	xcs.set_server(talk_base::SocketAddress(m_gtalkServer.c_str(), m_gtalkPort));

	talk_base::PhysicalSocketServer ss;


	jw->m_fileReceiveClient = new FileClient(pump.client(), buzz::JID_EMPTY, jw);
	jw->m_fileReceiveClient->SetCallBack(jw->m_cb );

    talk_base::Thread file_receive_thread(&ss);
    jw->m_fileReceiveThread = &file_receive_thread;
    talk_base::ThreadManager::SetCurrent(&file_receive_thread);

	pump.DoLogin(xcs, new XmppSocket(true), NULL);

	m_fileThreadInitialized = true;
	file_receive_thread.Run();
	m_fileThreadInitialized = false;
	pump.DoDisconnect();

	printf("m_fileReceiveThread returned\n");

	jw->m_fileReceiveThread = NULL;

    return NULL;
}

void CJingleWrapper::OnMessage(talk_base::Message *msg)
{
  switch (msg->message_id)
  {
	case CallClient::MSG_START:
		printf("CJingleWrapper::OnMessage() MSG_START branch called\n");
		assert(0);
		break;
	case CallClient::MSG_INPUT:
		talk_base::TypedMessageData<std::string> *data =
			static_cast<talk_base::TypedMessageData<std::string>*>(msg->pdata);
		m_client->ParseLine(data->data());
		break;
  }
}


void CJingleWrapper::connect(const std::string& username, const std::string& pwd)
{

    // This app has four threads.
    // One thread (thread_func) will run the XMPP client that does
    // the chat and the phone call xmpp communication.
    // On thread is responsible for file transfer.
    // A third thread is used by PhoneSessionClient as its worker thread.
    // The fourth thread is the GUI thread.

    m_usn = username;
    m_pwd = pwd;

    pthread_t thread_id;

    pthread_create(&thread_id, NULL, thread_func, this);
	while (m_callThreadInitialized == false)
		sleep(1);

	pthread_create(&thread_id, NULL, file_thread_func, this);
	while (m_fileThreadInitialized == false)
		sleep(1);
}

void CJingleWrapper::call(const std::string& friendAddress)
{
	m_mainThread->Post(this, CallClient::MSG_INPUT,
		            new talk_base::TypedMessageData<std::string>("call " + friendAddress + " "));
}

void CJingleWrapper::hangup()
{
	m_mainThread->Post(this, CallClient::MSG_INPUT,
		            new talk_base::TypedMessageData<std::string>("hangup "));
}

void CJingleWrapper::sendchat(const std::string& friendAddress, const std::string& text)
{
  m_mainThread->Post(this, CallClient::MSG_INPUT,
		            new talk_base::TypedMessageData<std::string>("sendchat " + friendAddress + " " + text + " "));
}

void CJingleWrapper::dingchat()
{
  m_mainThread->Post(this, CallClient::MSG_INPUT,
		            new talk_base::TypedMessageData<std::string>("dingchat "));
}
void CJingleWrapper::answerIncoming()
{
    m_mainThread->Post(this, CallClient::MSG_INPUT,
		            new talk_base::TypedMessageData<std::string>("accept "));
}

void CJingleWrapper::rejectIncoming()
{
    m_mainThread->Post(this, CallClient::MSG_INPUT,
		            new talk_base::TypedMessageData<std::string>("reject "));
}

void CJingleWrapper::sendfile(const std::string& contact, const std::string& path)
{
	m_fileReceiveThread->Post(m_fileReceiveClient, FileClient::MSG_SENDFILE,
					new talk_base::TypedMessageData<std::string>(contact + " " + path));
}


void CJingleWrapper::cancelFileTransfer()
{
	m_fileReceiveThread->Post(m_fileReceiveClient, FileClient::MSG_CANCELTRANSFER);

}

void CJingleWrapper::getSoundcardList(std::vector<std::string>& soundCards)
{
    soundCards.clear();
    
    MSList* cards = const_cast<MSList*>(ms_snd_card_manager_get_list(ms_snd_card_manager_get()));
    
    MSList *elem;
	for (elem=cards;elem!=NULL;elem=elem->next){
		MSSndCard *card=(MSSndCard*)elem->data;
        soundCards.push_back(ms_snd_card_get_string_id(card));
	}
}

void CJingleWrapper::selectSoundCards(const std::string& mic, const std::string& speaker)
{
    GlobalSettings::Instance()->setCards(mic, speaker);
}
