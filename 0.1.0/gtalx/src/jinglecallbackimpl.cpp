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

#include "jinglecallbackimpl.h"

#include "utils.h"
#include "main.h"

#include <qapplication.h>
#include <qwidget.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtextstream.h>

#include <vector>
#include <iostream>

#include <time.h>

#define TR_CONTEXT "LibjingleIf"

void CQTJingleCallbackImpl::EmitFoundOnlineContact(const std::string& contact)
{
	emit OnFoundOnlineContact(contact.c_str());
}

void CQTJingleCallbackImpl::EmitNewStatus(int statusId, const std::string& status)
{
	emit OnNewStatus(statusId, TR(status.c_str()));
}

void CQTJingleCallbackImpl::EmitContactStatus(const std::string& contact, const std::string& name, const std::string& status, int capabilities)
{
	emit OnContactStatus(contact.c_str(), name.c_str(), status.c_str(), capabilities);
}

void CQTJingleCallbackImpl::EmitError(int errId, const std::string& error)
{
	emit OnError(errId, TR(error.c_str()));
}

void CQTJingleCallbackImpl::EmitIncomingChat(const std::string& iconset,
											 const std::string& contact,
											 const std::string& text)
{
	emit OnIncomingChat(iconset.c_str(), QString::fromStdString(contact), QString::fromUtf8(text.c_str()));
}

void CQTJingleCallbackImpl::EmitIncomingCall(const std::string& contact)
{
       emit OnIncomingCall(contact.c_str());
}

void CQTJingleCallbackImpl::EmitCallDestroyed()
{
	emit OnCallDestroyed();
}


void CQTJingleCallbackImpl::EmitFileRecieveStart(const std::string& sender,
												 int fileSize,
												 const std::string& description,
												 std::string& rootDir)
{
	FileTransferStartData ftsd;
	emit OnFileReceiveStart(sender.c_str(), fileSize, description.c_str(), (long)&ftsd);

	while (ftsd.bDone == false)
	    sleep(1);

	if (ftsd.bAccept == true)
		rootDir = ftsd.sRootDir.toStdString();
	else
		rootDir = "";
}

void CQTJingleCallbackImpl::EmitFileTransferProgress(int total, int progress, std::string result)
{
	emit OnFileTransferProgress(total, progress, result.c_str());
}

CJingleCallbackImpl::CJingleCallbackImpl()
{
	QMutexLocker locker(&m_mutex);
}

CJingleCallbackImpl::~CJingleCallbackImpl()
{
	QMutexLocker locker(&m_mutex);

	if (m_wnd.size() != 0)
	{
		CUtils::log("At least one class did not unregister itself from CJingleCallbackImpl."
					"This can indicate a leak or might lead to crashes.");
	}
}

void CJingleCallbackImpl::addCBListener(CJingleCallback* wnd)
{
	QMutexLocker locker(&m_mutex);

	m_wnd.push_back(wnd);
}

void CJingleCallbackImpl::removeCBListener(CJingleCallback* wnd)
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.end();
	while (true)
	{
		--it;
		if (*it == wnd)
			m_wnd.erase(it);

		if (it == m_wnd.begin())
			break;
	}
}

void CJingleCallbackImpl::OnNewStatus(int statusId, const std::string& status)
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	while (it != m_wnd.end())
	{
		(*it)->OnNewStatus(statusId, status);
		++it;
	}
	m_QTCB.EmitNewStatus(statusId, status);
}

void CJingleCallbackImpl::OnContactStatus(const std::string& contact, const std::string& name, const std::string& status, int capabilities)
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	while (it != m_wnd.end())
	{
		(*it)->OnContactStatus(contact, name, status, capabilities);
		++it;
	}
	m_QTCB.EmitContactStatus(contact, name, status, capabilities);
}

void CJingleCallbackImpl::OnError(int errId, const std::string& error)
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	while (it != m_wnd.end())
	{
		(*it)->OnError(errId, error);
		++it;
	}
	m_QTCB.EmitError(errId, error);
}

void CJingleCallbackImpl::OnIncomingChat(const std::string& iconset,
                                         const std::string& contact,
                                         const std::string& text)
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	while (it != m_wnd.end())
	{
		(*it)->OnIncomingChat(iconset, contact, text);
		++it;
	}
	m_QTCB.EmitIncomingChat(iconset, contact, text);
}

void CJingleCallbackImpl::OnIncomingCall(const std::string& contact)
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	while (it != m_wnd.end())
	{
		(*it)->OnIncomingCall(contact);
		++it;
	}
	m_QTCB.EmitIncomingCall(contact);
}

void CJingleCallbackImpl::OnCallDestroyed()
{
	QMutexLocker locker(&m_mutex);

	std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	while (it != m_wnd.end())
	{
		(*it)->OnCallDestroyed();
		++it;
	}
	m_QTCB.EmitCallDestroyed();
}

void CJingleCallbackImpl::OnFileReceiveStart(const std::string& sender,
											 int fileSize,
											  const std::string& description,
											  std::string& rootDir)
{
	CUtils::log("file offered by " + QString::fromStdString(sender));

	QMutexLocker locker(&m_mutex);

	//std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	//while (it != m_wnd.end())
	//{
	//	(*it)->OnFileTransferStart(sender, rootDir);
	//	++it;
	//}
	m_QTCB.EmitFileRecieveStart(sender, fileSize, description, rootDir);

	CUtils::log("trying to save to " + QString::fromStdString(rootDir));
}

void CJingleCallbackImpl::OnFileTransferProgress(int total, int progress, std::string result)
{
	QMutexLocker locker(&m_mutex);

	//std::vector<CJingleCallback*>::iterator it = m_wnd.begin();
	//while (it != m_wnd.end())
	//{
	//	(*it)->OnFileTransferStart(sender, rootDir);
	//	++it;
	//}
	m_QTCB.EmitFileTransferProgress(total, progress, result);

	//CUtils::log("trying to save to " + QString::fromStdString(rootDir));

}

