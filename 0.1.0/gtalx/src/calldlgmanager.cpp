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

#include "calldlgmanager.h"
#include "main.h"
#include "jinglecallbackimpl.h"
#include "utils.h"
#include "trayiconmanager.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <qdir.h>

#define TR_CONTEXT "CallDlgManager"

CCallDlgManager::CCallDlgManager() :
    m_callDlgsVisible(true)
{
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnIncomingChat(QString, QString, QString)),
                      this, SLOT(OnIncomingChat(QString, QString, QString)));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnIncomingCall(QString)),
                      this, SLOT(OnIncomingCall(QString)));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnFileReceiveStart(QString, int, QString, long)),
                      this, SLOT(OnFileReceiveStart(QString, int, QString, long)));
}

CCallDlgManager::~CCallDlgManager()
{
	std::vector<DlgCall*>::iterator it = m_dlgs.end();
	
	while (it != m_dlgs.begin())
	{
		--it;
		delete (*it);
	}
}

void CCallDlgManager::RegisterDlg(DlgCall* dlg)
{
	m_dlgs.push_back(dlg);
}

void CCallDlgManager::UnregisterDlg(DlgCall* dlg)
{
	std::vector<DlgCall*>::iterator it = m_dlgs.begin();
	while (it != m_dlgs.end())
	{
		if ((*it) == dlg)
		{
			m_dlgs.erase(it);
			return;
		}
		
		++it;
	}
}

DlgCall* CCallDlgManager::DisplayDlg(QString contact, QWidget* parent, DlgCall::eReason reason)
{
	std::vector<DlgCall*>::iterator it = m_dlgs.begin();
	while (it != m_dlgs.end())
	{
		if (CUtils::AreContactsTheSame((*it)->m_contact, contact))
		{
			return *it;
		}
		
		++it;
	}
	
	DlgCall* dlg = new DlgCall(reason, contact, "", parent);
	SetupNewDlg(dlg);
	return dlg;
}

void CCallDlgManager::OnFileReceiveStart(QString sender, int fileSize, QString description, long ftsdPtr)
{
	// This function is implemented here because we want to know
	// when all open CallDlg instances have processed the 
	// event and the simplest is that the caller function waits
	// for the ftsd->bDone flag to be turned to true. Other options
	// would be much more complex.
	CQTJingleCallbackImpl::FileTransferStartData* ftsd = 
			(CQTJingleCallbackImpl::FileTransferStartData*)ftsdPtr;
			
	ftsd->bAccept = false;
	
	CUtils::log("CCallDlgManager::OnFileTransferStart");
	
	std::vector<DlgCall*>::iterator it;
	for (it = m_dlgs.begin();
		 it < m_dlgs.end();
		 ++it)
	{
		if ((*it)->OnFileOffered(sender, fileSize, description, ftsd->sRootDir))
		{
			if (ftsd->sRootDir.size() > 0)
				ftsd->bAccept = true;
			
			break;
		}
	}
	
	ftsd->bDone = true;
}

void CCallDlgManager::OnIncomingCall(QString contact)
{   
	contact = CUtils::getEmail(contact);
	
	if (QApplication::activeWindow() == NULL)
	{
		CAllContainer::m_trayIconManager->ShowTrayMessage(
			"gtalX", 
			QString(TR("Incoming call from %1")).arg(contact));
	}

	std::vector<DlgCall*>::iterator it = m_dlgs.begin();
	while (it != m_dlgs.end())
	{
		if (CUtils::AreContactsTheSame((*it)->m_contact, contact))
		{
			if (m_callDlgsVisible)
			{
				(*it)->show();
				QApplication::alert(*it);
			}
			return;
		}
		
		++it;
	}
	

	DlgCall* dlgCall = new DlgCall(DlgCall::eInCall, contact, "", CAllContainer::m_mainWnd);
	SetupNewDlg(dlgCall);
}

void CCallDlgManager::OnIncomingChat(QString iconset,
							 QString contact,
							 QString text)
{
	contact = CUtils::getEmail(contact);
	
	if (QApplication::activeWindow() == NULL)
	{
		QString shortText(text);
		if (shortText.length() > 23)
		{
			shortText.truncate(20);
			shortText += "...";
		}
		CAllContainer::m_trayIconManager->ShowTrayMessage(
			"gtalX", 
			QString(TR("%1 says: %2")).arg(contact).arg(QString("\n") + shortText));
		CAllContainer::m_libjingle->dingchat();
	}
	
	std::vector<DlgCall*>::iterator it = m_dlgs.begin();
	
	while (it != m_dlgs.end())
	{
		if (CUtils::AreContactsTheSame((*it)->m_contact, contact))
		{
			if (m_callDlgsVisible)
			{
				(*it)->show();
			}
			QApplication::alert(*it);
			return;
		}
			
		++it;
	}
	
	DlgCall* dlgCall = new DlgCall(DlgCall::eInChat, contact, text, CAllContainer::m_mainWnd);
	SetupNewDlg(dlgCall);
}

void CCallDlgManager::SetupNewDlg(DlgCall* dlgCall)
{
	SetDlgPos(dlgCall);
	dlgCall->setModal(false);
	dlgCall->setAttribute(Qt::WA_DeleteOnClose);

	if (m_callDlgsVisible)
	{
		dlgCall->show();
		QApplication::alert(dlgCall);
	}
}

void CCallDlgManager::SetDlgPos(DlgCall* dlgCall)
{
	if (m_dlgs.size() == 1)
		return;
		
	if (m_dlgs.size() == 2)
	{
		DlgCall* refDlg = m_dlgs[0];
		m_initialPos = refDlg->pos();
	}
	
	QPoint pt(m_initialPos);
	bool goodPosition = false;
	while (!goodPosition)
	{
		std::vector<DlgCall*>::iterator it = m_dlgs.end();
		goodPosition = true;
		while (it != m_dlgs.begin())
		{
			it--;
			
			if ((long)*it == (long)dlgCall)
				continue;
				
			if ((*it)->pos() == pt)
			{
				pt.setY(pt.y() + 20);
				pt.setX(pt.x() + 10);
				goodPosition = false;
				break;
			}
		}
	}
	dlgCall->move(pt);
}

void CCallDlgManager::SetAllDlgsVisible(bool visible)
{
	m_callDlgsVisible = visible;
	if (visible == false)
	{
		// in QT when we close a dialog it does not get deleted
		// now we want to keep only the dialogs that are visible
		std::vector<DlgCall*>::iterator it = m_dlgs.end();
		while (it != m_dlgs.begin())
		{
			it--;
			if (!(*it)->isVisible())
			{
				delete (*it);
				m_dlgs.erase(it);
			}
		}
	}
	std::vector<DlgCall*>::iterator it = m_dlgs.begin();
	while (it != m_dlgs.end())
	{
		(*it)->setVisible(visible);		
		++it;
	}
}

void CCallDlgManager::NotifyAllDlgsWeHungUp()
{
	std::vector<DlgCall*>::iterator it = m_dlgs.begin();
	
	while (it != m_dlgs.end())
	{
		(*it)->OnWeHungUp();		
		++it;
	}
}
