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

#ifndef __themesmanager__
#define __themesmanager__

#include <map>

#include <qstring.h>

class QWidget;

class CThemesManager 
{
public:
	CThemesManager();
	virtual ~CThemesManager();

	void applyStyleToWidget(QString cssFileName, QWidget* wnd);
	void getInstalledThemes(QStringList& themes, int& selectedThemeIndex);
	QString getActiveThemeDir(){return m_themeDir;}
	QString getThemeAuthor();
	
private:
	void readCssFile(QString cssFileName);
	
	QString m_themeDir;
	std::map< QString, QString > m_themeLines;
	std::map<QString, QString> m_Info;
};

#endif // __themesmanager__
