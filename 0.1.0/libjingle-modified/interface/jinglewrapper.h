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

#ifndef JINGLEWRAPPER_H_
#define JINGLEWRAPPER_H_

#include "libjingleif.h"

#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/xmpp/constants.h"
#include "talk/base/helpers.h"
#include "talk/base/thread.h"
#include "talk/base/network.h"
#include "talk/base/socketaddress.h"
#include "talk/base/logging.h"
#include "talk/p2p/base/sessionmanager.h"
#include "talk/p2p/client/httpportallocator.h"
#include "talk/p2p/client/sessionmanagertask.h"
#include "talk/session/phone/phonesessionclient.h"
#include "talk/login/jingleinfotask.h"
#include "talk/chat/chattask.h"


#include <mediastreamer2/mssndcard.h>

class ContactsManager;
class CallClient;
class FileClient;

namespace talk_base
{
    class Thread;
}

class CJingleWrapper : public CLibjingleIf, public talk_base::MessageHandler
{
public:
    CJingleWrapper( CJingleCallback* cb,
					const std::string& gtalkServer,
					int gtalkPort,
					const std::string& ringWav,
					const std::string& callWav,
					const std::string& chatDingWav,
					bool logVerbose,
				    const std::string& proxyHost,
				    int proxyPort); 
    virtual ~CJingleWrapper();

    virtual void connect(const std::string& usn, const std::string& pwd);   
    virtual void call(const std::string& friendAddress);
    virtual void hangup();
    virtual void sendchat(const std::string& friendAddress, const std::string& text);
	virtual void dingchat();
    
    virtual void answerIncoming();
    virtual void rejectIncoming();
	
	virtual void sendfile(const std::string& contact, const std::string& path);
	virtual void cancelFileTransfer();
    
    virtual void getSoundcardList(std::vector<std::string>& soundCards);
    virtual void selectSoundCards(const std::string& mic, const std::string& speaker);

	CallClient* m_client;
	FileClient* m_fileReceiveClient;
	ContactsManager* m_ContactsManager;
	
protected:
    void OnMessage(talk_base::Message *msg);
    
private:
    static void* thread_func(void* p);
	static void* file_thread_func(void* p);

    talk_base::Thread* m_fileReceiveThread;
    talk_base::Thread* m_mainThread;
    
    static std::string m_gtalkServer;
	static int m_gtalkPort;
	
    std::string m_ringWav;
    std::string m_callWav;
	std::string m_chatDingWav;
    std::string m_usn;
    std::string m_pwd;
    CJingleCallback* m_cb;
	bool m_logVerbose;
	
	static std::string m_proxyHost;
	static int m_proxyPort;
	static bool m_callThreadInitialized;
	static bool m_fileThreadInitialized;
};
#endif
