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

#include "dlgmain.h"

#include "main.h"
#include "jinglecallbackimpl.h"
#include "settingsmanager.h"
#include "utils.h"
#include "dlgcall.h"
#include "dlgabout.h"
#include "dlgsettings.h"
#include "themesmanager.h"
#include "calldlgmanager.h"
#include "trayiconmanager.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <qdesktopwidget.h>

#define TR_CONTEXT "MainDlg"

DlgMain::DlgMain(QWidget *parent)
    : QWidget(parent),
	m_bDoCLose(false)
{
	setObjectName("DlgMain");
    setWindowTitle(CAllContainer::m_gtalxVersionNumber);
	setWindowIcon(QIcon(CAllContainer::m_themesManager->getActiveThemeDir() + QString("icon_gtalx.png")));

    m_IdLoggedIn = new QLabel(this);
	m_IdLoggedIn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QPushButton* btSettings = new QPushButton;
	btSettings->setObjectName("btSettings");
    btSettings->setText(TR("Settings"));
    btSettings->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton* btAbout = new QPushButton;
	btAbout->setObjectName("btAbout");
    btAbout->setText(TR("About"));
    btAbout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QWidget* topLayoutHost = new QWidget;
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_IdLoggedIn);
	topLayout->addWidget(btSettings);
    topLayout->addWidget(btAbout);
    topLayoutHost->setLayout(topLayout);

    m_ContactsCtrl = new CContactsWidget(this);
	m_ContactsCtrl->setObjectName("m_ContactsCtrl");

    m_StatusBar = new QLabel(this);
	m_StatusBar->setObjectName("m_StatusBar");
    m_StatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLayoutHost);
    layout->addWidget(m_ContactsCtrl);
    layout->addWidget(m_StatusBar);
    setLayout(layout);

    resize(400, 250);

    QRect rect;
	rect = QApplication::desktop()->geometry();
	move(rect.center() - this->rect().center());

    connect( btAbout, SIGNAL( clicked() ), this, SLOT( btAbout_clicked() ) );
	connect( btSettings, SIGNAL( clicked() ), this, SLOT( btSettings_clicked() ) );
	connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnNewStatus(int, QString)),
            this, SLOT(OnNewStatus(int, QString)));
	connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnContactStatus(QString, QString, QString, int)),
            this, SLOT(OnContactStatus(QString, QString, QString, int)));
	connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnError(int, QString)),
            this, SLOT(OnError(int, QString)));
	connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnCallDestroyed()),
            this, SLOT(OnCallDestroyed()));

	CAllContainer::m_themesManager->applyStyleToWidget("maindlg.css", this);
}

DlgMain::~DlgMain()
{
    delete m_ContactsCtrl;
}

void DlgMain::OnNewStatus(int statusId, QString status)
{
	m_StatusBar->setText(QString(status));
}

void DlgMain::OnError(int errId, QString error)
{
	m_StatusBar->setText(error);
}

void DlgMain::OnContactStatus(QString contact, QString name, QString status, int capabilities)
{
	m_ContactsCtrl->UpdateContact(CUtils::getEmail(contact));
}

void DlgMain::OnCallDestroyed()
{
	m_StatusBar->setText(QString(TR("Call finished")));
}

void DlgMain::btAbout_clicked()
{
    DlgAbout dlg(this);
    dlg.exec();
}

void DlgMain::btSettings_clicked()
{
	DlgSettings dlg(this);
	dlg.exec();
}

void DlgMain::close()
{
	m_bDoCLose = true;
	QWidget::close();
}

void DlgMain::closeEvent(QCloseEvent* event)
{
	if (CAllContainer::m_trayIconManager->isTrayIconAvailable() &&
        !m_bDoCLose &&
        CAllContainer::m_sm["closetosystray"] != "0")
	{
		CAllContainer::m_callDlgManager->SetAllDlgsVisible(false);
		hide();
		event->ignore();
	}
}

void DlgMain::showEvent(QShowEvent* event)
{
	m_IdLoggedIn->setText(QString(TR("Hello %1")).arg(CAllContainer::m_email));
}
