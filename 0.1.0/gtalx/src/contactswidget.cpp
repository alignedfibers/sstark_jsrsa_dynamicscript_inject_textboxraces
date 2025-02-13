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

#include "contactswidget.h"
#include "main.h"
#include "jinglecallbackimpl.h"
#include "dlgcall.h"
#include "dlgmain.h"
#include "utils.h"
#include "contactsmanager.h"
#include "calldlgmanager.h"
#include "themesmanager.h"

#define TR_CONTEXT "ContactsWidget"

#include <qmessagebox.h>
#include <algorithm>
#include <assert.h>

CContactsWidget::CContactsWidget(QWidget* parent)
	: QScrollArea(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	m_vLayout = new QVBoxLayout;
	m_vLayout->setAlignment(Qt::AlignTop);
	m_vLayout->setSpacing(1);
	m_vLayout->setContentsMargins(1,1,1,1);
	// if you remove the next line, the vertical scrollbar wont work
	m_vLayout->setSizeConstraint(QLayout::SetMinimumSize);

	m_entryHost = new QLabel;
	setWidget(m_entryHost);
	m_entryHost->setLayout(m_vLayout);
	m_entryHost->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	m_ColorOdd = QColor("lightgray");
	m_ColorEven = QColor("lightblue");
    m_ColorOff = QColor("lightgray").lighter(110);

	if (CAllContainer::m_sm["mainDlgEvenCalleeBColor"] != "")
	{
		QColor col;
		col.setNamedColor(CAllContainer::m_sm["mainDlgEvenCalleeBColor"]);
		if (col.isValid())
		{
			m_ColorEven = col;
		}
	}
	if (CAllContainer::m_sm["mainDlgOddCalleeBColor"] != "")
	{
		QColor col;
		col.setNamedColor(CAllContainer::m_sm["mainDlgOddCalleeBColor"]);
		if (col.isValid())
		{
			m_ColorOdd = col;
		}
	}

    if (CAllContainer::m_sm["mainDlgOffCalleeBColor"] != "")
	{
		QColor col;
		col.setNamedColor(CAllContainer::m_sm["mainDlgOffCalleeBColor"]);
		if (col.isValid())
		{
			m_ColorOff = col;
		}
	}

	setWidgetResizable(true);

	std::vector<QString> emails;
	CAllContainer::m_contactsManager->GetEmailsList(emails);

    std::sort(emails.begin(), emails.end());

	for (std::vector<QString>::iterator it = emails.begin();
		 it != emails.end();
		 ++it)
	{
		UpdateContact(*it);
	}
}

CContactsWidget::~CContactsWidget()
{
}

void CContactsWidget::UpdateContact(const QString& email)
{
	if (email == CAllContainer::m_email)
		return;

    QString newStatus(CAllContainer::m_contactsManager->GetContactStatus(email));

	ContactWidgetMap::iterator it = m_ContactWidgetMap.find(email);
	if (it != m_ContactWidgetMap.end())
	{
        UpdateContactStatus(it->second, newStatus);
        return;
    }

    it = m_OfflineContactWidgetMap.find(email);
    if (it != m_OfflineContactWidgetMap.end())
    {
        UpdateContactStatus(it->second, newStatus);
        return;
    }
    UpdateContactStatus(new CContactEntryWidget(email,
                                                CAllContainer::m_contactsManager->GetContactName(email),
                                                m_entryHost), newStatus);
}

void CContactsWidget::UpdateContactStatus(CContactEntryWidget* widget,
                                          const QString& newStatus)
{
    QString oldStatus(widget->m_status);
    QString email(widget->m_email);
    widget->UpdateStatus(newStatus);

    if (oldStatus != "" &&
            (oldStatus == "offline" && newStatus == "offline" ||
            oldStatus != "offline" && newStatus != "offline" ))
    {
        // no big change
        return;
    }

    // We'll insert the widget to the right position
    m_vLayout->removeWidget(widget);

    if ((oldStatus == "offline" || oldStatus == "") && newStatus != "offline")
    {
        m_ContactWidgetMap[email] = widget;
        ContactWidgetMap::iterator it = m_OfflineContactWidgetMap.find(email);
        if (it != m_OfflineContactWidgetMap.end())
        {
            m_OfflineContactWidgetMap.erase(it);
        }
        m_vLayout->insertWidget(0, widget);
    }
    else if ((oldStatus != "offline" || oldStatus == "") && newStatus == "offline")
    {
        m_OfflineContactWidgetMap[email] = widget;
        ContactWidgetMap::iterator it = m_ContactWidgetMap.find(email);
        if (it != m_ContactWidgetMap.end())
        {
            m_ContactWidgetMap.erase(it);
        }
        m_vLayout->insertWidget(m_ContactWidgetMap.size(), widget);
    }
    else
    {
        assert(0);
    }

    SetEntriesBgColor();
}

void CContactsWidget::SetEntriesBgColor()
{
	ContactWidgetMap::iterator it = m_ContactWidgetMap.begin();
	while (it != m_ContactWidgetMap.end())
	{
		QPalette palette;
		if ((m_vLayout->indexOf(it->second) % 2 != 0) && m_ColorOdd.isValid())
		{
			palette.setColor(QPalette::Background, m_ColorOdd);
			it->second->setPalette(palette);
			it->second->setAutoFillBackground(true);
		}
		else if ((m_vLayout->indexOf(it->second) % 2 == 0) && m_ColorEven.isValid())
		{
			palette.setColor(QPalette::Background, m_ColorEven);
			it->second->setPalette(palette);
			it->second->setAutoFillBackground(true);
		}
		it++;
	}

    it = m_OfflineContactWidgetMap.begin();
    while (it != m_OfflineContactWidgetMap.end())
	{
		QPalette palette;
        palette.setColor(QPalette::Background, m_ColorOff);

        it->second->setPalette(palette);
        it->second->setAutoFillBackground(true);

		it++;
	}
}

CContactEntryWidget::CContactEntryWidget(const QString& email, const QString& name, QWidget* parent) :
	QWidget(parent),
	m_email(email),
    m_name(name)
{
	// create the button and the label
	m_btContact = new QPushButton;
	m_btContact->setText(TR("Contact"));
	m_btContact->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	m_lbEmail = new QLabel;
    if (CAllContainer::m_sm["contactNamesInsteadEmails"] != "0" &&
        m_name.size() > 0)
    {
        m_lbEmail->setText(m_name);
        m_lbEmail->setToolTip(m_email);
    }
    else
    {
        m_lbEmail->setText(m_email);
        m_lbEmail->setToolTip(m_name);
    }
	m_lbEmail->setAlignment(Qt::AlignVCenter);
	m_lbEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

	m_lbStatus = new QLabel;
	m_lbStatus->setMaximumSize(QSize(12, 12));
	m_lbStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_lbStatus->setScaledContents(true);

	// create a QHBoxLayout object that will resize/move the button & the label
	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->setAlignment(Qt::AlignVCenter);
	hLayout->addWidget(m_lbStatus);
	hLayout->addWidget(m_lbEmail);
	hLayout->addWidget(m_btContact);

	hLayout->setSpacing(3);
	//setContentsMargins(1,1,20,1);

	// add the QHBoxLayout onto the entry
	setLayout(hLayout);

	connect( m_btContact, SIGNAL( clicked() ), this, SLOT( buttonClicked() ) );

}

void CContactEntryWidget::buttonClicked()
{
	CAllContainer::m_callDlgManager->DisplayDlg(m_email, (QWidget*)parent(), DlgCall::eContact);
}

void CContactEntryWidget::UpdateStatus(QString status)
{
	QString text;

    m_status = status;
	//QString status(CAllContainer::m_contactsManager->GetContactStatus(m_email));
	if (status == "offline")
	{
		text = "off";
	}
	else if (status == "do not disturb")
	{
		text = "busy";
	}
	else if (status == "away")
	{
		text = "away";
		status = "idle";
	}
	else if (status == "online")
	{
		text = "online";
	}

	if (text != "off")
	{
		if (CAllContainer::m_contactsManager->IsContactCapableOf(m_email, CJingleCallback::callReady))
			text += "_p";

		m_btContact->setEnabled(true);
	}
	else
	{
		m_btContact->setEnabled(false);
	}

	m_lbStatus->setToolTip(TR(status));
    m_lbStatus->setPixmap(QPixmap(CAllContainer::m_themesManager->getActiveThemeDir() + "icon_contact_" + text + ".png"));

}
