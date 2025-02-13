#include "rostertask.h"

#include <iostream>

using namespace buzz;

RosterTask::RosterTask(Task * parent) :
	XmppTask(parent, XmppEngine::HL_TYPE)
{
	//std::cout << "RosterTask ctor"  << std::endl;

	XmlElement iq(QN_IQ);
	iq.AddAttr(QN_TYPE, "get");
	iq.AddAttr(QN_ID, "gtalx_rosterQ");
	iq.AddElement(new XmlElement(QN_ROSTER_QUERY, true));

	SendStanza(&iq);
}

RosterTask::~RosterTask()
{
}

int RosterTask::ProcessStart()
{
	return STATE_RESPONSE;
}

bool RosterTask::HandleStanza(const XmlElement * stanza)
{
	if (stanza->Name() == QN_IQ &&
	    stanza->HasAttr(QN_ID) && stanza->Attr(QN_ID) == "gtalx_rosterQ")
	{
		//std::cout << "gtalx_rosterQ " << stanza->Str().c_str() << std::endl;

	    const XmlChild* query = stanza->FirstChild();
		if (query == NULL ||
			query->AsElement() == NULL ||
			query->AsElement()->Name().Merged() != "jabber:iq:roster:query")
		{
			std::cout << "(query == NULL)" << std::endl;
			return false;
		}

	    const XmlChild* item = query->AsElement()->FirstChild();
		if (item == NULL || item->AsElement() == NULL)
		{
			std::cout << "(item == NULL) - no entries in list" << std::endl;
			return true;
		}
		do
		{
			std::string sContact(item->AsElement()->Attr(QName("jid")));
            std::string sName(item->AsElement()->Attr(QName("name")));

            RosterContactInfo rci;
            rci.m_Email = sContact;
            rci.m_Name = sName;
			m_contacts.push_back(rci);
		}
		while ((item = item->NextChild()) != NULL && item->AsElement() != NULL);

		RosterTaskResults(GetContacts());
		return true;
	}
	return false;
}
