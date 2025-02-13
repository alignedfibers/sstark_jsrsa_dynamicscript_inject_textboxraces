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

#ifndef _CHATTASK_H_
#define _CHATTASK_H_

#include "talk/xmpp/xmppengine.h"
#include "talk/xmpp/xmpptask.h"
#include "../../config.h"

#ifdef HAVE_MEDIASTREAMER28
class _MSFilter;
#endif

namespace buzz {
 
  
class ChatTask : public XmppTask {
public:
	ChatTask(Task * parent, std::string chat_ding_wav) : XmppTask(parent, XmppEngine::HL_TYPE) 
	{
		chat_ding_wav_ = chat_ding_wav;
		ding_finished_ = true;
		sndcard_ = NULL;
	}
	virtual ~ChatTask() {}


	sigslot::signal3<const std::string& /*iconset*/, 
				   const std::string& /*from*/, 
				   const std::string& /*text*/> SignalIncomingChat;
	void SendOutgoingChat(const std::string& str_to, const std::string& str_text);
	void DingChat();

protected:
	virtual int ProcessStart();
	virtual bool HandleStanza(const XmlElement * stanza);

	std::string chat_ding_wav_;
	
	#ifdef HAVE_MEDIASTREAMER28
	static void MSFilterNotifyFunc(void *userdata , _MSFilter*, unsigned int id, void *arg);
	#else
	static void MSFilterNotifyFunc(void *userdata , unsigned int id, void *arg);
	#endif

	bool ding_finished_;
	
	// MSSndCard *
	void* sndcard_;
};

}

#endif

