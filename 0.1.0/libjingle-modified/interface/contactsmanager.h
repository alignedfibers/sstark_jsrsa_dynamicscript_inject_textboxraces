#ifndef __CONTACTSMANAGER_H_
#define __CONTACTSMANAGER_H_

#include "talk/base/autodetectproxy.h"
#include "talk/p2p/base/session.h"
#include "talk/p2p/client/httpportallocator.h"
#include "talk/login/status.h"

namespace buzz
{
	class PresencePushTask;
	class PresenceOutTask;
	class RosterTask;
    struct RosterContactInfo;
};

struct RosterItem {
  buzz::Jid jid;
  buzz::Status::Show show;
  buzz::Status status;
  std::string name;
};

typedef std::map<std::string,RosterItem> RosterMap;

class CJingleCallback;

// This class keeps track of contacts - well rather sessions.
// If a person is logged in in 2 apps then roster_ contains
// 2 entries for the same user. Then if we get a request e.g.
// to call someone then we go through the roster_ and try
// to find an entry that matches the email address and has
// voip capabilities. see CallClient::MakeCallTo() for an
// example.
class ContactsManager : public sigslot::has_slots<>
{
public:
	ContactsManager(buzz::XmppClient *xmppclient, CJingleCallback* cb);
	virtual ~ContactsManager();

	void init();

	const RosterMap& getRoster(){return roster_;}

    buzz::Status myStatus_;

private:
	void OnStatusUpdate(const buzz::Status& status);
	void OnRosterList(const std::vector<buzz::RosterContactInfo>& contacts);
	const char* DescribeStatus(const buzz::Status& status);

	buzz::PresencePushTask* presence_push_;
	buzz::PresenceOutTask* presence_out_;
    buzz::PresenceOutTask* filetransfer_presence_out_;
	buzz::RosterTask* roster_task_;

	RosterMap roster_;
	CJingleCallback* cb_;
	buzz::XmppClient *xmpp_client_;
};



#endif
