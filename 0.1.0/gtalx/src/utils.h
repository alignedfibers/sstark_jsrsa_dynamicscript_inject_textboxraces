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

#ifndef UTILS_H
#define UTILS_H

#include <qstring.h>
#include <vector>

class QWidget;

class CUtils
{
    CUtils(){}

public: 
    static void setWidgetBackground(QWidget* widget, const QString& settingName, const QString& defCol);
    static void log(const QString& str);
	static QString getEmail(const QString& contact);
	static QString getUsn(const QString& contact);
	static bool AreContactsTheSame(const QString& contact1, const QString& contact2);
	static void ReadFileIntoVector(QString fileName, std::vector<QString>& Lines);
	static bool ReadSimpleSettingsFile(QString fileName, std::map<QString, QString>& settings);
	static QString FileSizeToString(unsigned int size);
};

#endif
