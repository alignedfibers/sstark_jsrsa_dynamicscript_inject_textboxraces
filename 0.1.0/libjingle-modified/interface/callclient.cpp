/*
 * Jingle call example
 * Copyright 2004--2005, Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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

#include <string>
#include <vector>

#include "errorhandler.h"
#include "callclient.h"
#include "jinglewrapper.h"
#include "contactsmanager.h"




#include <iostream>


CallClient::CallClient(buzz::XmppClient* xmpp_client, CJingleWrapper* jingw)
		: xmpp_client_(xmpp_client), /*roster_(new RosterMap),*/ call_(NULL),
		incoming_call_(false),
		cb_(NULL),
		jingw_(jingw)
{

	
	m_Statuses[CJingleCallback::eConnecting] = "Connecting";
	m_Statuses[CJingleCallback::eLoggingIn] = "Logging in";
	m_Statuses[CJingleCallback::eLoggedIn] = "Logged in";
	m_Statuses[CJingleCallback::eLoggedOut] = "Logged out";
	m_Statuses[CJingleCallback::eCalling] = "Calling";
	m_Statuses[CJingleCallback::eCallAnswered] = "Call answered";
	m_Statuses[CJingleCallback::eCallNotAnswered] = "Call not answered";
	m_Statuses[CJingleCallback::eCallInProgress] = "Call in progress";	
	m_Statuses[CJingleCallback::eOtherSideHungUp] = "Other side hung up";
	
	xmpp_client_->SignalStateChange.connect(this, &CallClient::OnStateChange);
}

CallClient::~CallClient()
{
	
}

void CallClient::ParseLine(const std::string& line)
{
	std::vector<std::string> words;
	int start = -1;
	int state = 0;
	for (int index = 0; index <= static_cast<int>(line.size()); ++index)
	{
		if (state == 0)
		{
			if (!isspace(line[index]))
			{
				start = index;
				state = 1;
			}
		}
		else
		{
			assert(state == 1);
			assert(start >= 0);
			if (isspace(line[index]))
			{
				std::string word(line, start, index - start);
				words.push_back(word);
				start = -1;
				state = 0;
			}
		}
	}
	
	if (words[0] == "dingchat")
	{
		DingChat();
	}
	else if ((words.size() > 2) && (words[0] == "sendchat"))
	{
		std::string text;
		for (int i=2; i<words.size(); i++)
		{
			text += words[i] + " ";
		}
		SendChatText(words[1], text);
	}
	else if (call_ && incoming_call_)
	{
		if ((words.size() == 1) && (words[0] == "accept"))
		{
			assert(call_->sessions().size() == 1);
			call_->AcceptSession(call_->sessions()[0]);
			phone_client()->SetFocus(call_);
			incoming_call_ = false;
		}
		else if ((words.size() == 1) && (words[0] == "reject"))
		{
			call_->RejectSession(call_->sessions()[0]);
			
			if (call_)
				call_->Terminate();
				
			call_ = NULL;
			incoming_call_ = false;
		}
		else
		{
			if (cb_) cb_->OnError(eInvalidRequest, ERR2STR(eInvalidRequest));
		}
	}
	else if (call_)
	{
		if ((words.size() == 1) && (words[0] == "hangup"))
		{
			if (call_)
				call_->Terminate();
				
			call_ = NULL;
			session_ = NULL;
			incoming_call_ = false;
		}
		else if ((words.size() == 1) && (words[0] == "mute"))
		{
			call_->Mute(true);
		}
		else if ((words.size() == 1) && (words[0] == "unmute"))
		{
			call_->Mute(false);
		}
		else
		{
			if (cb_) cb_->OnError(eInvalidRequest, ERR2STR(eInvalidRequest));
		}
	}
	else
	{
		if ((words.size() == 2) && (words[0] == "call"))
		{
			MakeCallTo(words[1]);
		}
		else
		{
			if (cb_) cb_->OnError(eInvalidRequest, ERR2STR(eInvalidRequest));
		}
	}
}



void CallClient::OnCallDestroy(cricket::Call* call)
{
	if (call == call_)
	{
		if (cb_)
			cb_->OnCallDestroyed();
			
		if (call_)
			call_->Terminate();
			
		call_ = NULL;
		session_ = NULL;
		incoming_call_ = false;
	}
}

void CallClient::OnJingleInfo(const std::string &relay_token,
                              const std::vector<std::string> &relay_addresses,
                              const std::vector<talk_base::SocketAddress> &stun_addresses)
{
	port_allocator_->SetStunHosts(stun_addresses);
	port_allocator_->SetRelayHosts(relay_addresses);
	port_allocator_->SetRelayToken(relay_token);
}

void CallClient::OnStateChange(buzz::XmppEngine::State state)
{
	switch (state)
	{
		case buzz::XmppEngine::STATE_START:
			if (cb_)
				cb_->OnNewStatus(CJingleCallback::eConnecting, StatusToStr(CJingleCallback::eConnecting));
			break;

		case buzz::XmppEngine::STATE_OPENING:
			if (cb_)
				cb_->OnNewStatus(CJingleCallback::eLoggingIn, StatusToStr(CJingleCallback::eLoggingIn));
			break;

		case buzz::XmppEngine::STATE_OPEN:
			if (cb_)
				cb_->OnNewStatus(CJingleCallback::eLoggedIn, StatusToStr(CJingleCallback::eLoggedIn));
			InitPhone();
			InitPresence();
			break;

		case buzz::XmppEngine::STATE_CLOSED:
			buzz::XmppEngine::Error error = xmpp_client_->GetError(NULL);
			if (cb_)
			{
				cb_->OnNewStatus(CJingleCallback::eLoggedOut, StatusToStr(CJingleCallback::eLoggedOut));
				cb_->OnError(error, ERR2STR(error));
			}
	}
}

void CallClient::InitPhone()
{
	std::string client_unique = xmpp_client_->jid().Str();
	cricket::InitRandom(client_unique.c_str(), client_unique.size());

	worker_thread_ = new talk_base::Thread();

	port_allocator_ = new cricket::HttpPortAllocator(&network_manager_, "call");

	session_manager_ = new cricket::SessionManager(
	    port_allocator_, worker_thread_);
	session_manager_->SignalRequestSignaling.connect(
	    this, &CallClient::OnRequestSignaling);
	session_manager_->OnSignalingReady();

	session_manager_task_ =
	    new cricket::SessionManagerTask(xmpp_client_, session_manager_);
	session_manager_task_->EnableOutgoingMessages();
	session_manager_task_->Start();

	buzz::JingleInfoTask *jit = new buzz::JingleInfoTask(xmpp_client_);
	jit->RefreshJingleInfoNow();
	jit->SignalJingleInfo.connect(this, &CallClient::OnJingleInfo);
	jit->Start();

	phone_client_ = new cricket::PhoneSessionClient(
	    xmpp_client_->jid(),session_manager_, ring_wav_, call_wav_);
	phone_client_->SignalCallCreate.connect(this, &CallClient::OnCallCreate);

	worker_thread_->Start();
}

void CallClient::OnRequestSignaling()
{
	session_manager_->OnSignalingReady();
}

void CallClient::OnCallCreate(cricket::Call* call)
{
	call->SignalSessionState.connect(this, &CallClient::OnSessionState);
}

void CallClient::OnSessionState(cricket::Call* call,
                                cricket::Session* session,
                                cricket::Session::State state)
{
	if (state == cricket::Session::STATE_RECEIVEDINITIATE)
	{
		buzz::Jid jid(session->remote_name());
		if (cb_)
		{
			cb_->OnIncomingCall(jid.Str());
		}
		call_ = call;
		session_ = session;
		incoming_call_ = true;
	}
	else if (state == cricket::Session::STATE_SENTINITIATE)
	{
		if (cb_)
			cb_->OnNewStatus(CJingleCallback::eCalling, StatusToStr(CJingleCallback::eCalling));
	}
	else if (state == cricket::Session::STATE_RECEIVEDACCEPT)
	{
		if (cb_)
			cb_->OnNewStatus(CJingleCallback::eCallAnswered, StatusToStr(CJingleCallback::eCallAnswered));
	}
	else if (state == cricket::Session::STATE_RECEIVEDREJECT)
	{
		if (call_)
			call_->Terminate();
			
		call_ = NULL;
		session_ = NULL;
		incoming_call_ = false;
			
		if (cb_)
			cb_->OnNewStatus(CJingleCallback::eCallNotAnswered, StatusToStr(CJingleCallback::eCallNotAnswered));
	}
	else if (state == cricket::Session::STATE_INPROGRESS)
	{
		if (cb_)
			cb_->OnNewStatus(CJingleCallback::eCallInProgress, StatusToStr(CJingleCallback::eCallInProgress));
	}
	else if (state == cricket::Session::STATE_RECEIVEDTERMINATE)
	{
		if (call_)
			call_->Terminate();
			
		call_ = NULL;
		session_ = NULL;
		incoming_call_ = false;
		
		if (cb_)
			cb_->OnNewStatus(CJingleCallback::eOtherSideHungUp, StatusToStr(CJingleCallback::eOtherSideHungUp));
	}
}

void CallClient::InitPresence()
{
	jingw_->m_ContactsManager->init();
	chat_task_ = new buzz::ChatTask(xmpp_client_, chat_ding_wav_);
	chat_task_->SignalIncomingChat.connect(this, &CallClient::OnIncomingChat);
}

void CallClient::OnIncomingChat(const std::string& iconset,
                                const std::string& from,
                                const std::string& text)
{
	cb_->OnIncomingChat(iconset, from, text);
}

void CallClient::MakeCallTo(const std::string& name)
{
	bool found = false;
	buzz::Jid found_jid;
	buzz::Jid callto_jid = buzz::Jid(name);
	RosterMap::const_iterator iter = jingw_->m_ContactsManager->getRoster().begin();
	while (iter != jingw_->m_ContactsManager->getRoster().end())
	{
		if (iter->second.jid.BareEquals(callto_jid) && iter->second.status.phone_capability())
		{
			found = true;
			found_jid = iter->second.jid;
			break;
		}
		++iter;
	}

	if (found)
	{
		phone_client()->SignalCallDestroy.connect(
		    this, &CallClient::OnCallDestroy);
		if (!call_)
		{
			call_ = phone_client()->CreateCall();
			if (cb_)
				cb_->OnNewStatus(CJingleCallback::eCallAnswered, StatusToStr(CJingleCallback::eCallAnswered));
			call_->SignalSessionState.connect(this, &CallClient::OnSessionState);
			session_ = call_->InitiateSession(found_jid, NULL);
		}
		phone_client()->SetFocus(call_);
	}
	else
	{
		if (cb_) cb_->OnError(eCannotFindContact, ERR2STR(eCannotFindContact));
	}
}

void CallClient::SendChatText(const std::string& emailTo, const std::string& txt)
{
	bool found = false;
	buzz::Jid jid = buzz::Jid(emailTo);
	RosterMap::const_iterator iter = jingw_->m_ContactsManager->getRoster().begin();
	while (iter != jingw_->m_ContactsManager->getRoster().end())
	{
		if (iter->second.jid.BareEquals(jid))
		{
			found = true;
			break;
		}
		++iter;
	}
	if (found)
	{
		chat_task_->SendOutgoingChat(emailTo, txt);
	}
	else
	{
		if (cb_) cb_->OnError(eCannotFindContact, ERR2STR(eCannotFindContact));
	}
}

void CallClient::DingChat()
{
	chat_task_->DingChat();
}

void CallClient::SetWavs(const std::string& ring_wav,
						 const std::string& call_wav,
						 const std::string& chat_ding_wav)
{
	ring_wav_ = ring_wav;
	call_wav_ = call_wav;
	chat_ding_wav_ = chat_ding_wav;
}

void CallClient::SetLog(bool verbose)
{
	if (verbose)
      talk_base::LogMessage::LogToDebug(talk_base::LS_VERBOSE);
}


const std::string CallClient::StatusToStr(int status)
{
	return m_Statuses[status];	
}
