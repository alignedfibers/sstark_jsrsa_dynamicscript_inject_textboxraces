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

#ifndef smileymanager_h_
#define smileymanager_h_

#include <qstring.h>
#include <map>

class CSmileysManager
{
public:
	CSmileysManager();
	virtual ~CSmileysManager();
	
	bool Init();
	QString FormatText(QString txtIn);
    void getInstalledSmileys(QStringList& smileys, int& selectedSmiley);
private:
	QString m_Path;
	std::map<QString, QString> m_Strings;
};

#endif
