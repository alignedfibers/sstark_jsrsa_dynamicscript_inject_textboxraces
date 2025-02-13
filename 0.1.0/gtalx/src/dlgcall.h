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


#ifndef DLGCALL_H
#define DLGCALL_H

#include "interface/libjingleif.h"

#include <qdialog.h>

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class CBEvent;
class QLabel;
class QTextBrowser;
class QLineEdit;
class QProgressBar;

class DlgCall : public QDialog
{
    Q_OBJECT

 public:
    enum eReason
	{
		eContact,
		eInCall,
		eInChat
	};
	
    DlgCall(eReason reason, QString contact, QString data, QWidget *parent = 0);
    virtual ~DlgCall();

	bool OnFileOffered(QString contact, int fileSize,
					   QString description, QString& dir);
    void OnWeHungUp();
	QString m_contact;
	
protected slots:
    void btAnswer_clicked();
    void btReject_clicked();
    void btHangUp_clicked();
    void btSendChat_clicked();
	void btCall_clicked();
	void btCancelFileTransfer_clicked();
	void btSendFile_clicked();
	
    void OnNewStatus(int statusId, QString status);
	void OnError(int errId, QString error);
    void OnIncomingChat(QString iconset,
                        QString contact,
                        QString text);
	void OnIncomingCall(QString contact);
	void OnCallDestroyed();
	
	void OnFileTransferProgress(int total, int progress, QString result);
	
protected:
    void closeEvent(QCloseEvent* event);
	
    // 1: incoming call 2: chat 3: in call + chat
    void setupUI(int reason);
	void ClearHLayout();
	void addChatText(QString text, QString from);
	
	void MarkLinks(QString& text);
	// checks that in the string only the following characters are present:
	// numbers, letters, :, /, .
	// string is checked until lastPos
	bool ContainsOnlyDomainCharacters(QString link, int lastPos);
    
    QPushButton* m_btSendChat;
    QPushButton* m_btHangUp;
	QPushButton* m_btCall;
	QPushButton* m_btSendFile;
    QPushButton* m_btAnswer;
    QPushButton* m_btReject;
	QPushButton* m_btCancelFileTransfer;
	QProgressBar* m_pbFileTransfer;

    QTextBrowser* m_tbShowChat;
    QLineEdit* m_ebSendChat;
    
	QWidget* m_sendChatHolder;
    QWidget* m_topButtonsHolder;
	QWidget* m_topLayoutHolder;
	QWidget* m_fileTransferCtrlsHolder;
	QHBoxLayout* m_hLayoutChatSend;
    QHBoxLayout* m_hTopButtonsLayout;
	QHBoxLayout* m_hFileTransferCtrlsLayout;
    QVBoxLayout* m_vLayout;
	QGridLayout* m_gTopLayout;
    
    QLabel* m_lbInfo;
	QLabel* m_lbImage;
    
    bool m_bRinging;
	bool m_bCallInProgress;
	bool m_bFileTransferInProgress;
};


#endif
