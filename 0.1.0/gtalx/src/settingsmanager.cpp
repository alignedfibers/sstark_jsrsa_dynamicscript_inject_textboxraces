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

#include "settingsmanager.h"
#include "utils.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>

#include <iostream>

CSettingsManager::CSettingsManager(const QString& settingsFileName) :
    m_FileName(QDir::homePath() + "/.gtalx/settings.txt")
{
	// defaults
	m_settings["lastEmail"] = "example@gmail.com";
	m_settings["SmileyIcons"] = "default";
	m_settings["GtalkServer"] = "talk.google.com";
	m_settings["GtalkPort"] = "5222";

    QFile f(m_FileName);
	if ( ! f.open ( QIODevice::ReadOnly ) )
	{
		QDir dir;
        dir.mkdir(QDir::homePath() + "/.gtalx");

        return;
	}
	QTextStream stream ( &f );
	QString sLine;

    int iComment = 0;
	while ( ! stream.atEnd() )
	{
		sLine = stream.readLine();

		//CUtils::log("setting: " + sLine);

		if (sLine[0] == '#' || sLine.size() == 0)
		{
			m_settings["#" + QString::number(iComment)] = sLine;
			m_unorderedSettings.push_back("#" + QString::number(iComment));
			++iComment;
			continue;
		}

		if (sLine.contains('=') == 0)
		{
		    continue;
		}

		m_settings[sLine.section('=', 0, 0).simplified()] =
		                                 sLine.section('=', 1, 1).simplified();
		m_unorderedSettings.push_back(sLine.section('=', 0, 0).simplified());
    }
}


CSettingsManager::~CSettingsManager()
{
}

const QString CSettingsManager::operator[] (const QString& key)
{
    QString val(m_settings[key]);
    return val;
}

void CSettingsManager::SetSetting(const QString& key, const QString& val)
{
    m_settings[key] = val;
}

bool CSettingsManager::saveSettings()
{
    QFile f(m_FileName);
	if ( ! f.open ( QIODevice::WriteOnly ) )
	{
	    //std::cerr << "cannot open settings-file for writing " << m_FileName << std::endl;
	    return false;
	}

    // making a copy because this list will be destroyed as we save the settings
    std::map<QString, QString> currentSettings = m_settings;

	std::vector<QString>::iterator it = m_unorderedSettings.begin();
	QTextStream stream( &f );
    while (it != m_unorderedSettings.end())
    {
    	QString sLine;
    	if (it->left(1) == "#")
    		sLine = currentSettings[*it];
        else
        	sLine = *it + "=" + currentSettings[*it];

        if (currentSettings.find(*it) != currentSettings.end())
            currentSettings.erase(currentSettings.find(*it));

        if (sLine.size() > 0)
        {
            stream << sLine << "\n";
        }
        it++;
    }

    stream << "\n";

    std::map<QString, QString>::iterator it2 = currentSettings.begin();
    while (it2 != currentSettings.end())
    {
        QString sLine;
        sLine = it2->first + "=" + it2->second;
        if (sLine.size() > 0)
        {
            stream << sLine << "\n";
        }
        it2++;
    }

    f.close();
    return true;
}
