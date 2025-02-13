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

#include "themesmanager.h"
#include "utils.h"
#include "main.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qwidget.h>

CThemesManager::CThemesManager()
{
	if (CAllContainer::m_sm["ActiveTheme"] == "")
		CAllContainer::m_sm.SetSetting("ActiveTheme", "default");

	m_themeDir = CAllContainer::m_sharePath + "themes/" + QString(CAllContainer::m_sm["ActiveTheme"]);

	if (!m_themeDir.endsWith("/"))
	    m_themeDir += "/";

	CUtils::ReadSimpleSettingsFile(m_themeDir + "info.txt", m_Info);

	m_themeLines.clear();

	readCssFile("connectdlg.css");
	readCssFile("maindlg.css");
	readCssFile("calldlg.css");
}

CThemesManager::~CThemesManager()
{
}

void CThemesManager::readCssFile(QString cssFileName)
{
	QFile f(m_themeDir + cssFileName);
	if ( ! f.open ( QIODevice::ReadOnly ) )
		return;

	CUtils::log("Reading theme file: " + m_themeDir + cssFileName);

	QTextStream stream ( &f );
	QString sLine;

	m_themeLines[cssFileName] = "";

	while ( ! stream.atEnd() )
	{
		sLine = stream.readLine();
		sLine = sLine.trimmed();

	    if (sLine.count() == 0)
			continue;

		if (sLine.left(1) == "#")
			continue;

		m_themeLines[cssFileName] += sLine + "\n";
	}
}

void CThemesManager::applyStyleToWidget(QString cssFileName, QWidget* wnd)
{
	wnd->setStyleSheet(m_themeLines[cssFileName]);
}

void CThemesManager::getInstalledThemes(QStringList& themes, int& selectedThemeIndex)
{
	QString themeDir(CAllContainer::m_sharePath + "themes/");

	QDir dir(themeDir);

    dir.setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);

	selectedThemeIndex = -1;

	themes = dir.entryList();

	int idx = 0;
	for (QStringList::iterator it = themes.begin(); it<themes.end(); ++it)
	{
	    if ((*it) == CAllContainer::m_sm["ActiveTheme"])
		{
			selectedThemeIndex = idx;
			break;
		}
		++idx;
	}
}

QString CThemesManager::getThemeAuthor()
{
	return m_Info["author"];
}
