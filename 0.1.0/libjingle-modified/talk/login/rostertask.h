#ifndef __rostertask__
#define __rostertask__

#include "talk/xmpp/xmppengine.h"
#include "talk/xmpp/xmpptask.h"
#include "talk/base/sigslot.h"
#include "talk/login/status.h"

#include <vector>

namespace buzz {

struct RosterContactInfo
{
    std::string m_Email;
    std::string m_Name;
};

class RosterTask : public XmppTask
{

public:
	RosterTask(Task * parent);
	virtual ~RosterTask();

	sigslot::signal1<const std::vector<RosterContactInfo>&> RosterTaskResults;

	const std::vector<RosterContactInfo>& GetContacts()
	{
		return m_contacts;
	}
	virtual int ProcessStart();

protected:
    virtual bool HandleStanza(const XmlElement * stanza);

	std::vector<RosterContactInfo> m_contacts;
};

}

#endif // __rostertask__
