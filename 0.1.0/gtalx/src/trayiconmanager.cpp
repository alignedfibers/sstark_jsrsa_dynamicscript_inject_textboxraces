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
#include "trayiconmanager.h"
#include "calldlgmanager.h"
#include "dlgmain.h"

#include <qmenu.h>
#include <qmessagebox.h>

#include "main.h"

#define TR_CONTEXT "TrayIcon"

CTrayIconManager::CTrayIconManager(DlgMain* mainWnd)
{
	if (!isTrayIconAvailable())
	    return;

	m_mainWnd = mainWnd;

	m_trayIconMenu = new QMenu(mainWnd);

	createActions();

	m_icon.addFile(CAllContainer::m_themesManager->getActiveThemeDir() + QString("icon_gtalx.png"));

	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setContextMenu(m_trayIconMenu);
	m_trayIcon->setIcon(m_icon);
	m_trayIcon->setVisible(true);

	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(activated(QSystemTrayIcon::ActivationReason)));

	connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));

}

CTrayIconManager::~CTrayIconManager()
{
	m_trayIcon->setVisible(false);
	delete m_trayIconMenu;
	delete m_trayIcon;
	delete m_quitAction;
}

bool CTrayIconManager::isTrayIconAvailable() const
{
    try
    {
        return QSystemTrayIcon::isSystemTrayAvailable();
    }
    catch(...)
    {
        return false;
    }
}

void CTrayIconManager::createActions()
{
	m_quitAction = new QAction(TR("&Quit"), this);
	connect(m_quitAction, SIGNAL(triggered()), this, SLOT(quit()));
	m_trayIconMenu->addAction(m_quitAction);
}

void CTrayIconManager::ShowTrayMessage(QString title, QString message)
{
   m_trayIcon->showMessage(title, message, QSystemTrayIcon::Information);
}

void CTrayIconManager::quit()
{
	m_mainWnd->close();
}

void CTrayIconManager::activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		if (m_mainWnd->isMinimized() || !m_mainWnd->isVisible())
		{
			m_mainWnd->showNormal();
			CAllContainer::m_callDlgManager->SetAllDlgsVisible(true);
		}
		else
		{
			CAllContainer::m_callDlgManager->SetAllDlgsVisible(false);
			m_mainWnd->hide();
		}
	}
}

void CTrayIconManager::messageClicked()
{
	m_mainWnd->showNormal();
	m_mainWnd->raise();
	m_mainWnd->setFocus();
	CAllContainer::m_callDlgManager->SetAllDlgsVisible(true);
}


