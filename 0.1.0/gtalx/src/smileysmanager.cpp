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

#include "smileysmanager.h"
#include "main.h"
#include "utils.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>

CSmileysManager::CSmileysManager()
{
	Init();
}

CSmileysManager::~CSmileysManager()
{
}

bool CSmileysManager::Init()
{
	if (CAllContainer::m_sm["SmileyIcons"] == "")
		CAllContainer::m_sm.SetSetting("SmileyIcons", "default");

	m_Path = CAllContainer::m_sharePath + "smileys/" + CAllContainer::m_sm["SmileyIcons"];

	m_Strings.clear();

	QString stringsFile(m_Path + "/_strings.txt");


	QFile f(stringsFile);
	if ( ! f.open ( QIODevice::ReadOnly ) )
	{
		CUtils::log("smileys file not found: " + stringsFile);
        return false;
	}

	CUtils::log("smileys taken from: " + stringsFile);

	QTextStream stream( &f );
	QString sLine;
	QString sSection;

	while ( ! stream.atEnd() )
	{
		sLine = stream.readLine();
		sLine = sLine.trimmed();

		if (sLine[0] == '#' || sLine.size() == 0)
		{
			continue;
		}

		if (sLine.contains("\"=\"") == 0)
		{
		    CUtils::log("unrecognised line in smileys file: " + sLine);
		    continue;
		}

		QString key(sLine.section("\"=\"", 0, 0).simplified());
		QString val(sLine.section("\"=\"", 1, 1).simplified());
		key.remove(0, 1);
		val.chop(1);

		// the editbox will require this, so we'll get text in which these replacements have been made
		// so we do it in the strings read from the file so that the text we get will match the text
		// in the file.
		val.replace("&", "&amp;");
		val.replace("<", "&lt;");
		val.replace(">", "&gt;");

		// since a filename cannot contain = but the chat string can contain anything, it was easier
		// to specify that the filename is the key in the file. Since we still search for the string
		// and not the filename, it is useful to have the string as key, so in this class the val
		// will be the key and the key will be the value.
		m_Strings[val] = key;
    }
    return true;
}

QString CSmileysManager::FormatText(QString txtIn)
{
	for (std::map<QString, QString>::iterator it = m_Strings.begin();
		 it != m_Strings.end();
		 ++it)
	{
		//http://www.smileyarena.com/
		txtIn.replace(it->first, " <IMG SRC=\"" + m_Path + "/" + it->second + "\" WIDTH=15 HEIGHT=15>");
	}
	return txtIn;
}

void CSmileysManager::getInstalledSmileys(QStringList& smileys, int& selectedSmiley)
{
	QString smileyDir(CAllContainer::m_sharePath + "smileys/");

	QDir dir(smileyDir);
    dir.setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);

	smileys = dir.entryList();

	int idx = 0;
    QString selectedSmileys(CAllContainer::m_sm["SmileyIcons"]);
	for (QStringList::iterator it = smileys.begin(); it<smileys.end(); ++it)
	{
	    if ((*it) == selectedSmileys)
		{
			selectedSmiley = idx;
			break;
		}
		++idx;
	}
}
