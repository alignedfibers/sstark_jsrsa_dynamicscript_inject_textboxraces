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


#include "chattask.h"

#include "talk/xmpp/constants.h"

#include <mediastreamer2/mediastream.h>
#include <mediastreamer2/mssndcard.h>

#include <iostream>
#include <time.h>

namespace buzz {

bool ChatTask::HandleStanza(const XmlElement * stanza)
{
    if (stanza->Attr(QN_TYPE) != "chat")
    {
        return true;
    }

    if (!stanza->HasAttr(QN_FROM))
    {
        return true;
    }

	if (!stanza->FirstNamed(QN_BODY))
	{
		return true;
	}
	
    SignalIncomingChat("", stanza->Attr(QN_FROM), stanza->FirstNamed(QN_BODY)->BodyText());
    return false;
}

void ChatTask::SendOutgoingChat(const std::string& str_to, const std::string& str_text)
{
	XmlElement* presence = new XmlElement(QN_MESSAGE);
	presence->AddAttr(QN_TO, str_to);
	presence->AddAttr(QN_TYPE, "chat");

	XmlElement* body = presence->FindOrAddNamedChild(QN_BODY);
	body->SetBodyText(str_text);

	SendStanza(presence);
	delete presence;
}

int ChatTask::ProcessStart()
{
	return STATE_RESPONSE;
}

void ChatTask::DingChat()
{
	if (chat_ding_wav_.size() > 0 && sndcard_ == NULL)
	{
		RingStream* ding_stream = NULL;
		
		sndcard_=ms_snd_card_manager_get_default_card(ms_snd_card_manager_get());
		if (sndcard_)
		{
			ding_finished_ = false;
			ding_stream = ring_start_with_cb(chat_ding_wav_.c_str(), -2, 
											 (MSSndCard*)sndcard_,
											 MSFilterNotifyFunc, this);
			while (ding_finished_ == false)
			{
				timespec req;
				req.tv_sec = 0;
				req.tv_nsec = 200000000;
				nanosleep(&req, &req);
			}
			ring_stop(ding_stream);
			sndcard_ = NULL;
		}
	}
}

#ifdef HAVE_MEDIASTREAMER28
void ChatTask::MSFilterNotifyFunc(void *userdata , _MSFilter*, unsigned int id, void *arg)
#else
void ChatTask::MSFilterNotifyFunc(void *userdata , unsigned int id, void *arg)
#endif
{
	((ChatTask*)userdata)->ding_finished_ = true;
}
}

