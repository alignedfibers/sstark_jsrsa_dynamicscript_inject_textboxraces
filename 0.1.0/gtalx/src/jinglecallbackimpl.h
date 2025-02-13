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

#ifndef LIBJINGLECALLBACKIMPL_H_
#define LIBJINGLECALLBACKIMPL_H_

#include <vector>
#include <string>
#include <qstringlist.h>
#include <qevent.h>
#include <qmutex.h>

#include "interface/libjingleif.h"

//http://doc.trolltech.com/4.4/signalsandslots.html

class CQTJingleCallbackImpl : public QObject
{
	Q_OBJECT

public:

    virtual ~CQTJingleCallbackImpl(){}

	void EmitFoundOnlineContact(const std::string& contact);
	void EmitNewStatus(int statusId, const std::string& status);
    void EmitContactStatus(const std::string& contact, const std::string& name, const std::string& status, int capabilities);
    void EmitRosterList(const std::string& contact, const std::string& status, bool bMore);
	void EmitError(int errId, const std::string& error);
    void EmitIncomingChat(const std::string& iconset,
                        const std::string& contact,
                        const std::string& text);
	void EmitIncomingCall(const std::string& contact);
	void EmitCallDestroyed();
	void EmitFileRecieveStart(const std::string& sender,
							  int fileSize,
							  const std::string& description,
							  std::string& rootDir);
	void EmitFileTransferProgress(int total, int progress, std::string result);

	struct FileTransferStartData
	{
		FileTransferStartData() : bDone(false), bAccept(false){}
		QString sRootDir;
		bool bDone;
		bool bAccept;
	};

public slots:
signals:
	void OnFoundOnlineContact(QString contact);
	void OnNewStatus(int statusId, QString status);
    void OnContactStatus(QString contact, QString name, QString status, int capabilities);
	void OnError(int errId, QString error);
    void OnIncomingChat(QString iconset,
                        QString contact,
                        QString text);
	void OnIncomingCall(QString contact);
	void OnCallDestroyed();

	void OnFileReceiveStart(QString sender, int fileSize, QString description, long ftsd);
	void OnFileTransferProgress(int total, int progress, QString result);
};

class CJingleCallbackImpl : public CJingleCallback
{

public:
    CJingleCallbackImpl();
    virtual ~CJingleCallbackImpl();

	// these might be useful later
	// but we use CQTJingleCallbackImpl for now instead
    void addCBListener(CJingleCallback* wnd);
	void removeCBListener(CJingleCallback* wnd);

	void OnFoundOnlineContact(const std::string& contact);
	void OnNewStatus(int statusId, const std::string& status);
	void OnContactStatus(const std::string& contact, const std::string& name, const std::string& status, int capabilities);
    void OnRosterList(const std::string& contact, const std::string& status, bool bMore);
	void OnError(int errId, const std::string& error);
    void OnIncomingChat(const std::string& iconset,
                        const std::string& contact,
                        const std::string& text);
	void OnIncomingCall(const std::string& contact);
	void OnCallDestroyed();

	void OnFileReceiveStart(const std::string& sender,
							int fileSize,
							 const std::string& description,
							 std::string& rootDir);
	void OnFileTransferProgress(int total, int progress, std::string result);

	CQTJingleCallbackImpl m_QTCB;
protected:
    std::vector<CJingleCallback*> m_wnd;

	QMutex m_mutex;
};

#endif
