#include <iostream>

#include "talk/xmpp/xmppclient.h"

#include "talk/login/presencepushtask.h"
#include "talk/login/presenceouttask.h"
#include "talk/login/rostertask.h"

#include "libjingleif.h"
#include "contactsmanager.h"

ContactsManager::ContactsManager(buzz::XmppClient *xmppclient, CJingleCallback* cb) :
  xmpp_client_(xmppclient),
  cb_(cb)
{
}

ContactsManager::~ContactsManager()
{
	delete presence_out_;
	presence_out_ = NULL;
}

void ContactsManager::init()
{
	roster_task_ = new buzz::RosterTask(xmpp_client_);
	roster_task_->RosterTaskResults.connect(
	    this, &ContactsManager::OnRosterList);
	//roster_task_->Start();

	presence_push_ = new buzz::PresencePushTask(xmpp_client_);
	presence_push_->SignalStatusUpdate.connect(
	    this, &ContactsManager::OnStatusUpdate);
	presence_push_->Start();

	//buzz::Status my_status;
	myStatus_.set_jid(xmpp_client_->jid());
	myStatus_.set_available(true);
	myStatus_.set_show(buzz::Status::SHOW_ONLINE);
	myStatus_.set_priority(0);
	myStatus_.set_know_capabilities(true);
	myStatus_.set_phone_capability(true);
	myStatus_.set_is_google_client(true);
	myStatus_.set_fileshare_capability(false);
	myStatus_.set_version("1.0.0.66");

	presence_out_ =
	    new buzz::PresenceOutTask(xmpp_client_);
	presence_out_->Send(myStatus_);
	presence_out_->Start();
}

void ContactsManager::OnStatusUpdate(const buzz::Status& status)
{
    std::string key = status.jid().Str();

    if (roster_.find(key) == roster_.end())
    {
        RosterItem item;
        roster_[key] = item;
        roster_[key].jid = status.jid();
    }

	roster_[key].show = status.show();
	roster_[key].status = status;

	int capabilities = 0;
	capabilities |= CJingleCallback::chatReady;

	if (status.phone_capability())
	{
		capabilities |= CJingleCallback::callReady;
	}
	if (status.fileshare_capability())
	{
		capabilities |= CJingleCallback::fileshareReady;
	}

	if (cb_)
		cb_->OnContactStatus(key, roster_[key].name, DescribeStatus(roster_[key].status), capabilities);
}

void ContactsManager::OnRosterList(const std::vector<buzz::RosterContactInfo>& contacts)
{
	buzz::Status status;
	status.set_show(buzz::Status::SHOW_OFFLINE);

	for (std::vector<buzz::RosterContactInfo>::const_iterator it = contacts.begin();
		 it != contacts.end();
		 ++it)
	{
		RosterItem item;
		item.jid = buzz::Jid(it->m_Email);
		item.show = buzz::Status::SHOW_OFFLINE;
		item.status = status;
        item.name = it->m_Name;

		roster_[it->m_Email] = item;

		if (cb_)
		{
			cb_->OnContactStatus(it->m_Email, it->m_Name, DescribeStatus(item.status), 0);
		}
	}
}

const char* ContactsManager::DescribeStatus(const buzz::Status& status)
{
	switch (status.show())
	{
		case buzz::Status::SHOW_XA:
			return "XA";
		case buzz::Status::SHOW_ONLINE:
			return "online";
		case buzz::Status::SHOW_AWAY:
			return "away";
		case buzz::Status::SHOW_DND:
			return "do not disturb";
		case buzz::Status::SHOW_CHAT:
			return "chat";
		default:
			return "offline";
	}
	return "offline";
}

