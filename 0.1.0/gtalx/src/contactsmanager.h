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

#ifndef __ccontactsmanager__
#define __ccontactsmanager__

#include <qstring.h>
#include <qmutex.h>
#include <qobject.h>

#include <map>
#include <vector>

#include "interface/libjingleif.h"

class CContactsManager : public CJingleCallback
{
public:
	CContactsManager();
	virtual ~CContactsManager();

/*	void AddContactInfo(QString contact, QString status, int capabilities);
	void RemoveContactInfo(QString contact, QString status, int capabilities);
*/
    QString GetContactName(const QString& email);
	QString GetContactStatus(const QString& email);
	int GetContactCapabilities(const QString& email);
	bool IsContactCapableOf(const QString& email, CJingleCallback::eContactCapabilities capability);
	void GetEmailsList(std::vector<QString>& emails);

	void OnContactStatus(const std::string& contact, const std::string& name, const std::string& status, int capabilities);

private:
	struct ContactStatusInfo
	{
        QString name;
		QString status;
		int capabilities;
	};
	typedef std::map<QString, ContactStatusInfo> EmailContactStatusMap;
	std::map<QString, EmailContactStatusMap> m_emailStatusList;

	QMutex m_mutex;

	std::vector<QString> m_StatusPriorities;
};

#endif // __ccontactsmanager__
