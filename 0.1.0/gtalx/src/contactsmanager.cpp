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

#include "contactsmanager.h"

#include "jinglecallbackimpl.h"
#include "main.h"
#include "utils.h"


CContactsManager::CContactsManager()
{
	QMutexLocker locker(&m_mutex);

	CAllContainer::m_cbimpl->addCBListener(this);

	m_StatusPriorities.clear();
	m_StatusPriorities.push_back("unknown");
	m_StatusPriorities.push_back("offline");
	m_StatusPriorities.push_back("XA");
	m_StatusPriorities.push_back("do not disturb");
	m_StatusPriorities.push_back("away");
	m_StatusPriorities.push_back("chat");
	m_StatusPriorities.push_back("online");
}

CContactsManager::~CContactsManager()
{
	QMutexLocker locker(&m_mutex);

	CAllContainer::m_cbimpl->removeCBListener(this);
}
/*
void CContactsManager::AddContactInfo(QString contact, QString status, int capabilities)
{
	QMutexLocker locker(&m_mutex);
}

void CContactsManager::RemoveContactInfo(QString contact, QString status, int capabilities)
{
	QMutexLocker locker(&m_mutex);
}*/

QString CContactsManager::GetContactStatus(const QString& email)
{
	QMutexLocker locker(&m_mutex);

	EmailContactStatusMap& contacts = m_emailStatusList[email];
	int topStatusIdx = 0;
	EmailContactStatusMap::iterator itContact = contacts.begin();
	while (itContact != contacts.end())
	{
		for (int i=0; i<m_StatusPriorities.size(); ++i)
		{
			if ((m_StatusPriorities[i] == itContact->second.status))
			{
				if (i > topStatusIdx)
					topStatusIdx = i;

				break;
			}

		}
		++itContact;
	}
	return m_StatusPriorities[topStatusIdx];
}

QString CContactsManager::GetContactName(const QString& email)
{
    QMutexLocker locker(&m_mutex);

    EmailContactStatusMap& contacts = m_emailStatusList[email];
    if (contacts.size() > 0)
    {
        return contacts.begin()->second.name;
    }
    return "";
}

int CContactsManager::GetContactCapabilities(const QString& email)
{
	QMutexLocker locker(&m_mutex);

	int capabilities = 0;

	EmailContactStatusMap& contacts = m_emailStatusList[email];
	int topStatusIdx = 0;
	EmailContactStatusMap::iterator itContact = contacts.begin();
	while (itContact != contacts.end())
	{
		capabilities |= itContact->second.capabilities;
		++itContact;
	}
	return capabilities;
}

bool CContactsManager::IsContactCapableOf(const QString& email, CJingleCallback::eContactCapabilities capability)
{
	QMutexLocker locker(&m_mutex);

	int capabilities = 0;

	EmailContactStatusMap& contacts = m_emailStatusList[email];
	int topStatusIdx = 0;
	EmailContactStatusMap::iterator itContact = contacts.begin();
	while (itContact != contacts.end())
	{
		if ((capability & itContact->second.capabilities) != 0)
			return true;

		++itContact;
	}
	return false;
}

void CContactsManager::OnContactStatus(const std::string& contact, const std::string& name, const std::string& status, int capabilities)
{
	QMutexLocker locker(&m_mutex);

	QString email(CUtils::getEmail(contact.c_str()));
	EmailContactStatusMap& csm = m_emailStatusList[email];

    csm[contact.c_str()].name = QString::fromUtf8(name.c_str());
	csm[contact.c_str()].status = status.c_str();
	csm[contact.c_str()].capabilities = capabilities;
}

void CContactsManager::GetEmailsList(std::vector<QString>& emails)
{
	QMutexLocker locker(&m_mutex);

	emails.clear();

	for (std::map<QString, EmailContactStatusMap>::iterator it = m_emailStatusList.begin();
		 it != m_emailStatusList.end();
		 ++it)
	{
		CUtils::log("CContactsManager::GetEmailsList() " + it->first);
		emails.push_back(it->first);
	}
}
