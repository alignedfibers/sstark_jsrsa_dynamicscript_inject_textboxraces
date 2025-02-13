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

#include "main.h"
#include "utils.h"

#include <qwidget.h>
#include <qfile.h>
#include <qthread.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <iostream>

void CUtils::setWidgetBackground(QWidget* widget, const QString& settingName, const QString& defCol)
{
	QPushButton* bt = dynamic_cast<QPushButton*>(widget);
	QLineEdit* le = dynamic_cast<QLineEdit*>(widget);
	QTextEdit* te = dynamic_cast<QTextEdit*>(widget);

	QColor col;
	if (CAllContainer::m_sm[settingName] != "")
	{
		col.setNamedColor(CAllContainer::m_sm[settingName]);
	}

	if (!col.isValid())
	{
		col.setNamedColor(defCol);
	}

	QPalette palette;
	if (bt)
	    palette.setColor(QPalette::Button, col);
	else if (le || te)
	    palette.setColor(QPalette::Base, col);
	else
	    palette.setColor(QPalette::Background, col);

	widget->setPalette(palette);
	widget->setAutoFillBackground(true);
}

void CUtils::log(const QString& str)
{
	std::cout << str.toLocal8Bit().constData() << std::endl;
}

QString CUtils::getEmail(const QString& contact)
{
	if (contact.contains("/"))
		return contact.section('/', 0, 0);
	else
		return contact;
}

QString CUtils::getUsn(const QString& contact)
{
	if (contact.contains("@"))
		return contact.section('@', 0, 0);
	else
		return contact;
}

bool CUtils::AreContactsTheSame(const QString& contact1, const QString& contact2)
{
	QString s1(getEmail(contact1));
	QString s2(getEmail(contact2));

	s1 = s1.toLower();
	s2 = s2.toLower();

	s1 = s1.replace(".", "");
	s2 = s2.replace(".", "");

	return (s1 == s2);
}

void CUtils::ReadFileIntoVector(QString fileName, std::vector<QString>& Lines)
{
	QFile f(fileName);

	if ( ! f.open ( QIODevice::ReadOnly ) )
	{
		CUtils::log(fileName + " file not found");
		return;
	}

	QTextStream stream ( &f );
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

		Lines.push_back(sLine);
	}
}

bool CUtils::ReadSimpleSettingsFile(QString fileName, std::map<QString, QString>& settings)
{
	settings.clear();

	QFile f(fileName);
	if ( ! f.open ( QIODevice::ReadOnly ) )
	{
		CUtils::log("file not found: " + fileName);
        return false;
	}

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
		    CUtils::log("unrecognised line in " + fileName + ": " + sLine);
		    continue;
		}

		QString key(sLine.section("\"=\"", 0, 0).simplified());
		QString val(sLine.section("\"=\"", 1, 1).simplified());
		key.remove(0, 1);
		val.chop(1);

		settings[key] = val;
    }
	return true;
}

QString CUtils::FileSizeToString(unsigned int size)
{
    double size_display;
    QString format;
    QString ret;

    // the comparisons to 1000 * (2^(n10)) are intentional
    // it's so you don't see something like "1023 bytes",
    // instead you'll see ".9 KB"

    if (size < 1000)
	{
		format = "Bytes";
		size_display = size;
    }
	else if (size < 1000 * 1024)
	{
		format = "KiB";
		size_display = (double)size / 1024.0;
    }
	else if (size < 1000 * 1024 * 1024)
	{
		format = "MiB";
		size_display = (double)size / (1024.0 * 1024.0);
    }
	else
	{
		format = "GiB";
		size_display = (double)size / (1024.0 * 1024.0 * 1024.0);
    }
    return QString("%1 %2").arg(size_display).arg(format);
}
