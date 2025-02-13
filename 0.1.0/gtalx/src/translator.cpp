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

#include "translator.h"
#include "utils.h"
#include "main.h"

#include <stdlib.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qlocale.h>

const QString CTranslator::m_defaultLanguage = "English";

CTranslator::CTranslator()
{
	QString lang(CAllContainer::m_sm["language"]);
	if (lang != "")
    {
		CUtils::log("Selected language (from ~/.gtalx/settings.txt): " + lang);
 		changeLanguage(lang);
		return;
	}

	QLocale ql = QLocale::system();
	QLocale::Language l = ql.language ();

	lang = QLocale::languageToString (l);

	CUtils::log("Selected language (read from env. var. LANG): " + lang);

	changeLanguage(lang);
}

CTranslator::~CTranslator()
{
}

QString CTranslator::translate(const QString& context, const QString& text)
{
	if (m_lang.size() == 0)
		return text;

	QString txt(m_translations[context][text]);
	if (txt.size() == 0)
	{
		CUtils::log("language file does not define: " + context + " " + text);
		return text;
	}
	else
		return txt;
}

bool CTranslator::changeLanguage(QString language)
{
	if (language == m_defaultLanguage)
	    language = "";

	m_lang = language;
	QString languageFile =
		CAllContainer::m_sharePath + "languagefiles/" + m_lang + ".txt";

	m_translations.clear();

	if (language.size() == 0)
		return true;

	QFile f(languageFile);
	if ( ! f.open ( QIODevice::ReadOnly ) )
	{
		CUtils::log("language file not found: " + languageFile);
		CUtils::log("Translating gtalX is easy, have a look at the homepage "
					"and see how to do it. We would be happy to get and include "
					"your translation to any languages that have not been done yet.");

		m_lang = "";
        return false;
	}

	CUtils::log("applying language file: " + languageFile);

	QTextStream stream ( &f );
	stream.setCodec("UTF-8");
	stream.setAutoDetectUnicode (true);
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

		if (sLine[0] == '[' && sLine.endsWith(']'))
		{
			sSection = sLine.mid(1, sLine.size()-2);
			Items items;
			m_translations[sSection] = items;
			continue;
		}
		if (sLine.contains("\"=\"") == 0)
		{
		    CUtils::log("unrecognised line in language file: " + sLine);
		    continue;
		}

		QString key(sLine.section("\"=\"", 0, 0).simplified());
		QString val(sLine.section("\"=\"", 1, 1).simplified());
		key.remove(0, 1);
		val.chop(1);

		m_translations[sSection][key] = val;
    }
    return true;
}

void CTranslator::getInstalledLanguages(QStringList& languages, int& selectedLanguageIndex)
{
	QDir dir(CAllContainer::m_sharePath + "languagefiles/");

	QStringList filters;
    filters << "*.txt";
    dir.setNameFilters(filters);

	languages = dir.entryList();

	// idx is one because we're gonna insert English as the first item in the list
	selectedLanguageIndex = 0;
	int idx = 1;
	for (QStringList::iterator it = languages.begin(); it<languages.end(); ++it)
	{
		(*it).chop(4);
		if ((*it) == getCurrentLanguage())
		{
			selectedLanguageIndex = idx;
		}
		++idx;
	}
	languages.insert(languages.begin(), m_defaultLanguage);
}

QString CTranslator::getCurrentLanguage()
{
	if (m_lang == "")
	    return m_defaultLanguage;

	return m_lang;
}

