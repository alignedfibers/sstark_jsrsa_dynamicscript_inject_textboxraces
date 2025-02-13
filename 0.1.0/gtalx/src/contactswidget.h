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


#ifndef CONTACTSWIDGET_H_
#define CONTACTSWIDGET_H_

#include <qscrollarea.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <QResizeEvent>

#include <map>
#include <vector>

class CContactEntryWidget;

//typedef std::vector< std::pair<QString, CContactEntryWidget*> > ContactWidgetMap;
typedef std::map<QString, CContactEntryWidget*> ContactWidgetMap;

class CContactsWidget : public QScrollArea
{


public:
	CContactsWidget(QWidget* parent);
	virtual ~CContactsWidget();

	void UpdateContact(const QString& email);

	void SetEntriesBgColor();

	QColor m_ColorOdd;
	QColor m_ColorEven;
    QColor m_ColorOff;

private:
    void UpdateContactStatus(CContactEntryWidget* widget, const QString& newStatus);

    QVBoxLayout* m_vLayout;
    QWidget* m_entryHost;

    ContactWidgetMap m_ContactWidgetMap;
    ContactWidgetMap m_OfflineContactWidgetMap;

};



class CContactEntryWidget : public QWidget
{
	Q_OBJECT

public:
	CContactEntryWidget(const QString& email, const QString& name, QWidget* parent);

	void UpdateStatus(QString status);

    QString m_status;
	QString m_email;
    QString m_name;

protected slots:
	virtual void buttonClicked();

private:
    QPushButton* m_btContact;
	QLabel* m_lbEmail;
	QLabel* m_lbStatus;
};

#endif
