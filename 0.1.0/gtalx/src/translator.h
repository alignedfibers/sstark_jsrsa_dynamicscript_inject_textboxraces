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

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <map>
#include <vector>

#include <qstring.h>
#include <qstringlist.h>

class CTranslator
{
public:
    CTranslator();
    virtual ~CTranslator();

    QString translate(const QString& context, const QString& text);
	bool changeLanguage(QString language);
	void getInstalledLanguages(QStringList& languages, int& selectedLanguageIndex);
	QString getCurrentLanguage();

    static const QString m_defaultLanguage;
protected:
    QString m_lang;
    
    typedef std::map<QString, QString> Items;
    typedef std::map<QString, Items> Sections;
    Sections m_translations;
};



#endif
