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

#include "smileysmanager.h"
#include "dlgcall.h"

#include "main.h"
#include "jinglecallbackimpl.h"
#include "utils.h"
#include "calldlgmanager.h"
#include "contactsmanager.h"
#include "themesmanager.h"

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextbrowser.h>
#include <qstringlist.h>
#include <qfiledialog.h>
#include <qprogressbar.h>
#include <qdir.h>

#include <vector>

#define TR_CONTEXT "CallDlg"

DlgCall::DlgCall(eReason reason, QString contact, QString data, QWidget *parent)
		: QDialog(parent),
		m_bRinging(false),
		m_bCallInProgress(false),
		m_bFileTransferInProgress(false)
{
	m_contact = CUtils::getEmail(contact);

	CAllContainer::m_callDlgManager->RegisterDlg(this);

	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnNewStatus(int, QString)),
                      this, SLOT(OnNewStatus(int, QString)));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnError(int, QString)),
                      this, SLOT(OnError(int, QString)));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnIncomingChat(QString, QString, QString)),
                      this, SLOT(OnIncomingChat(QString, QString, QString)));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnIncomingCall(QString)),
                      this, SLOT(OnIncomingCall(QString)));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnCallDestroyed()),
                      this, SLOT(OnCallDestroyed()));
	QObject::connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnFileTransferProgress(int, int, QString)),
					  this, SLOT(OnFileTransferProgress(int, int, QString)));

	setWindowTitle(CAllContainer::m_gtalxVersionNumber + " - " + m_contact);

	resize(400, 200);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	// ****************
	// Creating controls
	// ****************

	m_tbShowChat = new QTextBrowser(this);
	m_tbShowChat->setObjectName("m_tbShowChat");
	m_tbShowChat->setTextInteractionFlags(Qt::TextSelectableByMouse |
											Qt::LinksAccessibleByMouse |
											Qt::LinksAccessibleByKeyboard);
	m_tbShowChat->setOpenExternalLinks(true);

	m_ebSendChat = new QLineEdit(this);
	m_ebSendChat->setObjectName("m_ebSendChat");

	m_lbInfo = new QLabel(this);
	m_lbInfo->setText(CUtils::getEmail(contact));
	m_lbInfo->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
	m_lbInfo->setObjectName("m_lbInfo");

	m_btAnswer = new QPushButton;
	m_btAnswer->setText(TR("Answer"));
	m_btAnswer->setAutoDefault(false);
	m_btAnswer->setObjectName("m_btAnswer");
	connect( m_btAnswer, SIGNAL( clicked() ), this, SLOT( btAnswer_clicked() ) );

	m_btReject = new QPushButton;
	m_btReject->setText(TR("Reject"));
	m_btReject->setAutoDefault(false);
	m_btReject->setObjectName("m_btReject");
	connect( m_btReject, SIGNAL( clicked() ), this, SLOT( btReject_clicked() ) );

	m_btHangUp = new QPushButton;
	m_btHangUp->setText(TR("Hang up"));
	m_btHangUp->setAutoDefault(false);
	m_btHangUp->setObjectName("m_btHangUp");
	connect( m_btHangUp, SIGNAL( clicked() ), this, SLOT( btHangUp_clicked() ) );

	m_btCall = new QPushButton;
	m_btCall->setText(TR("Call"));
	m_btCall->setAutoDefault(false);
	m_btCall->setObjectName("m_btCall");
	connect( m_btCall, SIGNAL( clicked() ), this, SLOT( btCall_clicked() ) );

	m_btSendFile = new QPushButton;
	m_btSendFile->setText(TR("Send file"));
	m_btSendFile->setAutoDefault(false);
	m_btSendFile->setObjectName("m_btSendFile");
	connect( m_btSendFile, SIGNAL( clicked() ), this, SLOT( btSendFile_clicked() ) );

	m_btSendChat = new QPushButton;
	m_btSendChat->setText(TR("Send"));
	m_btSendChat->setDefault(true);
	m_btSendChat->setObjectName("m_btSendChat");
	connect( m_btSendChat, SIGNAL( clicked() ), this, SLOT( btSendChat_clicked() ) );

	m_btCancelFileTransfer = new QPushButton;
	m_btCancelFileTransfer->setText(TR("Cancel"));
	m_btCancelFileTransfer->setAutoDefault(false);
	m_btCancelFileTransfer->setObjectName("m_btCancelFileTransfer");
	connect( m_btCancelFileTransfer, SIGNAL( clicked() ), this, SLOT( btCancelFileTransfer_clicked() ) );

	m_pbFileTransfer = new QProgressBar;

	m_lbImage = new QLabel(this);
 	m_lbImage->setMaximumSize(QSize(110, 66));
    m_lbImage->setPixmap(CAllContainer::m_themesManager->getActiveThemeDir() + QString("pic_calldlg.png"));
    m_lbImage->setScaledContents(true);

	// ****************
	// Creating layouts
	// ****************

	m_vLayout = new QVBoxLayout;
	m_vLayout->setAlignment(Qt::AlignTop|Qt::AlignHCenter);

	m_hTopButtonsLayout = new QHBoxLayout;
	m_hTopButtonsLayout->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
	m_hTopButtonsLayout->setSpacing(1);
	m_hTopButtonsLayout->setContentsMargins(1,1,1,1);

	m_topButtonsHolder = new QWidget;
	m_topButtonsHolder->setLayout(m_hTopButtonsLayout);

	m_hFileTransferCtrlsLayout = new QHBoxLayout;
	m_hFileTransferCtrlsLayout->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
	m_hFileTransferCtrlsLayout->setSpacing(1);
	m_hFileTransferCtrlsLayout->setContentsMargins(1,1,1,1);

	m_fileTransferCtrlsHolder = new QWidget;
	m_fileTransferCtrlsHolder->setLayout(m_hFileTransferCtrlsLayout);

	m_hLayoutChatSend = new QHBoxLayout;
	m_hLayoutChatSend->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
	m_hLayoutChatSend->setSpacing(1);
	m_hLayoutChatSend->setContentsMargins(1,1,1,1);

	m_sendChatHolder = new QWidget;
	m_sendChatHolder->setLayout(m_hLayoutChatSend);

	// *************************
	// adding widgets to layouts
	// *************************

	m_hLayoutChatSend->addWidget(m_ebSendChat);
	m_hLayoutChatSend->addWidget(m_btSendChat);

	m_hFileTransferCtrlsLayout->addWidget(m_btCancelFileTransfer);
	m_hFileTransferCtrlsLayout->addWidget(m_pbFileTransfer);

	m_gTopLayout = new QGridLayout;
	m_gTopLayout->setAlignment(Qt::AlignLeft);
	m_gTopLayout->setSpacing(1);
	m_gTopLayout->setContentsMargins(1,1,1,1);

	m_topLayoutHolder = new QWidget;
	m_topLayoutHolder->setLayout(m_gTopLayout);

	m_gTopLayout->addWidget(m_topButtonsHolder, 0, 0);
	m_gTopLayout->addWidget(m_lbInfo, 1, 0);
	m_gTopLayout->addWidget(m_lbImage, 0, 2, 2, 1);
	m_gTopLayout->setColumnStretch ( 1, 1 );

	m_vLayout->addWidget(m_topLayoutHolder);
	m_vLayout->addWidget(m_tbShowChat);
	m_vLayout->addWidget(m_sendChatHolder);

	if (reason == eInCall)
	{
		m_bRinging = true;
		setupUI(1);
	}
	else if (reason == eInChat)
	{
		m_lbInfo->setText(contact);
		addChatText(data, contact);
		setupUI(2);
	}
	else
	{
		setupUI(2);
	}

	setLayout(m_vLayout);

	CAllContainer::m_themesManager->applyStyleToWidget("calldlg.css", this);

	m_ebSendChat->setFocus();

}


DlgCall::~DlgCall()
{
	ClearHLayout();

	delete m_btHangUp;
	delete m_btAnswer;
	delete m_btReject;

	delete m_vLayout;

	CAllContainer::m_callDlgManager->UnregisterDlg(this);
}

void DlgCall::closeEvent(QCloseEvent* event)
{
	if (m_bRinging)
	{
		CAllContainer::m_libjingle->rejectIncoming();
	}

	if (m_bCallInProgress)
	{
		if (QMessageBox::question(
				this, TR("Warning"),
				QString(TR("If you close this dialog your voice-chat session will finish. "
						   "Do you want to continue?")),
				QMessageBox::Yes|QMessageBox::No)
			!= QMessageBox::Yes)
		{
			event->ignore();
			return;
		}
		CAllContainer::m_libjingle->hangup();
	}
	event->accept();
}

// 1: incoming call 2: chat 3: in call + chat
void DlgCall::setupUI(int reason)
{
	ClearHLayout();
	if (reason == 1)
	{
		m_lbInfo->setText(QString(TR("Incoming call from %1")).arg(m_contact));
		m_hTopButtonsLayout->addWidget(m_btAnswer);
		m_hTopButtonsLayout->addWidget(m_btReject);
	}
	else
	if (reason == 2)
	{
		m_lbInfo->setText(m_contact);
		m_hTopButtonsLayout->addWidget(m_btCall);
		m_hTopButtonsLayout->addWidget(m_btSendFile);
		m_btCall->setEnabled(true);
		m_btSendFile->setEnabled(true);
	}
	else
	if (reason == 3)
	{
		if (m_bCallInProgress)
		{
			m_lbInfo->setText(QString(TR("In call with %1")).arg(m_contact));
			m_hTopButtonsLayout->addWidget(m_btHangUp);
			m_hTopButtonsLayout->addWidget(m_btSendFile);
		}
		else
		{
			m_hTopButtonsLayout->addWidget(m_btCall);
			m_btCall->setEnabled(false);
			m_hTopButtonsLayout->addWidget(m_btSendFile);
			m_btSendFile->setEnabled(false);
		}
	}
	m_ebSendChat->setFocus();
}

void DlgCall::ClearHLayout()
{
	if (m_hTopButtonsLayout->indexOf(m_btCall) > -1)
	{
		m_hTopButtonsLayout->removeWidget(m_btCall);
		m_btCall->setParent(NULL);
	}

	if (m_hTopButtonsLayout->indexOf(m_btHangUp) > -1)
	{
		m_hTopButtonsLayout->removeWidget(m_btHangUp);
		m_btHangUp->setParent(NULL);
	}

	if (m_hTopButtonsLayout->indexOf(m_btAnswer) > -1)
	{
		m_hTopButtonsLayout->removeWidget(m_btAnswer);
		m_btAnswer->setParent(NULL);
	}

	if (m_hTopButtonsLayout->indexOf(m_btReject) > -1)
	{
		m_hTopButtonsLayout->removeWidget(m_btReject);
		m_btReject->setParent(NULL);
	}

	if (m_hTopButtonsLayout->indexOf(m_btSendFile) > -1)
	{
		m_hTopButtonsLayout->removeWidget(m_btSendFile);
		m_btSendFile->setParent(NULL);
	}

	m_topButtonsHolder->setLayout(m_hTopButtonsLayout);

}

void DlgCall::addChatText(QString text, QString from)
{
	if (from.size() > 0)
	{
		from = CUtils::getUsn(from);

		text.replace("&", "&amp;");
		text.replace("<", "&lt;");
		text.replace(">", "&gt;");

		text = CAllContainer::m_smileysManager->FormatText(text);

		MarkLinks(text);

		m_tbShowChat->append("<b>" + from + "</b>: " + text + "\n");
	}
	else
	{
	    m_tbShowChat->append(text + "\n");
	}
}

void DlgCall::MarkLinks(QString& text)
{
	std::vector<QString> DomainList;
	CUtils::ReadFileIntoVector(CAllContainer::m_sharePath + "domains.txt", DomainList);

	QStringList listOfWords;
	listOfWords = text.split(" ", QString::SkipEmptyParts);

	QString word;

	for (QStringList::iterator it = listOfWords.begin(); it < listOfWords.end(); ++it)
	{
		word = *it;

		if (word.endsWith(".") || word.endsWith(",") || word.endsWith(":") ||
			word.endsWith(";") || word.endsWith("!") || word.endsWith("?"))
		{
			word.chop(1);
		}

		for (std::vector<QString>::iterator itd = DomainList.begin(); itd < DomainList.end(); ++itd)
		{
			if (word.contains("." + *itd + "/", Qt::CaseInsensitive) ||
				word.endsWith("." + *itd, Qt::CaseInsensitive))
			{
				int pos = word.indexOf("." + *itd, 0, Qt::CaseInsensitive);

				if (ContainsOnlyDomainCharacters(word, pos))
				{
					text.replace(word,
						"<A HREF=\"http://" + (*it).replace("http://","") + "\">" + word + "</a>");
				}
			}
		}
	}
}

bool DlgCall::ContainsOnlyDomainCharacters(QString link, int lastPos)
{
	QString shortLink = link.left(lastPos);

	for (int i = 0; i< shortLink.size(); ++i)
	{
		if (shortLink[i].isLetterOrNumber() || shortLink[i] == '.' ||
			shortLink[i] == '/' || shortLink[i] == ':')
		{
			continue;
		}
		return false;
	}
	return true;
}



void DlgCall::btAnswer_clicked()
{
	ClearHLayout();
	m_hTopButtonsLayout->addWidget(m_btHangUp);

	CAllContainer::m_libjingle->answerIncoming();

	m_bCallInProgress = true;
	setupUI(3);

	m_bRinging = false;
}

void DlgCall::btReject_clicked()
{
	CAllContainer::m_libjingle->rejectIncoming();
	setupUI(2);
	m_bRinging = false;
}

void DlgCall::btSendChat_clicked()
{
	if (m_ebSendChat->text().size() == 0)
		return;

	CAllContainer::m_libjingle->sendchat(m_contact.toStdString(), (const char*)m_ebSendChat->text().toUtf8());
	addChatText(m_ebSendChat->text(), CAllContainer::m_email);
	m_ebSendChat->setText("");

	if (CAllContainer::m_contactsManager->GetContactStatus(m_contact) == "offline")
	{
		QString msg(QString(TR("%1 is offline, he might not have received your message.")).arg(m_contact));
		addChatText(QString("<FONT COLOR=\"#FF0000\">") + msg + QString("</FONT>"), "");
	}
}

void DlgCall::btHangUp_clicked()
{
	CAllContainer::m_libjingle->hangup();
	CAllContainer::m_callDlgManager->NotifyAllDlgsWeHungUp();
}

void DlgCall::btCall_clicked()
{
	if (!CAllContainer::m_contactsManager->IsContactCapableOf(m_contact, CJingleCallback::callReady))
	{
		QMessageBox::information(
			this, TR("Warning"),
			QString(TR("You cannot call %1 because he uses a chat application "
					   "that is not capable of voice chat.")).arg(m_contact));
		return;
	}
	CAllContainer::m_libjingle->call(m_contact.toStdString());
	m_bCallInProgress=true;
	setupUI(3);
	m_lbInfo->setText(QString(TR("Calling %1")).arg(m_contact));
}

void DlgCall::btSendFile_clicked()
{
    if (m_bFileTransferInProgress)
    {
        QMessageBox::information(
			this, TR("Warning"),
			QString(TR("File transfer is in progress. Only one file can be transfered at a time.")));
		return;
    }
	if (!CAllContainer::m_contactsManager->IsContactCapableOf(m_contact, CJingleCallback::fileshareReady))
	{
		QMessageBox::information(
			this, TR("Warning"),
			QString(TR("You cannot send files to %1 because he uses a chat application "
					   "that is not capable of receiving files.")).arg(m_contact));
		return;
	}

	QString dir;
	dir = QFileDialog::getOpenFileName(this,
									   TR("Please select the file or directory that you want to send"),
								       QDir::home().path());
	if (dir.size() > 0)
	{
		m_bFileTransferInProgress = true;
		addChatText(TR("Waiting for the other party to accept"), "");
		CAllContainer::m_libjingle->sendfile(m_contact.toStdString(), dir.toStdString());
	}
}

void DlgCall::OnNewStatus(int statusId, QString status)
{
	if (statusId == CJingleCallback::eCallInProgress)
	{
		setupUI(3);
	}
	else if (statusId == CJingleCallback::eCallAnswered)
	{
	}
	else if (statusId == CJingleCallback::eCallNotAnswered)
	{
		m_bCallInProgress = false;
		setupUI(2);
	}
	else if (statusId == CJingleCallback::eOtherSideHungUp)
	{
		m_bCallInProgress = false;
		setupUI(2);
	}
}

void DlgCall::OnError(int errId, QString error)
{
	m_lbInfo->setText(error);
}

void DlgCall::OnIncomingChat(QString iconset,
                             QString contact,
                             QString text)
{
	if (CUtils::AreContactsTheSame(contact, m_contact))
	{
		addChatText(text, m_contact);
	}
}

void DlgCall::OnIncomingCall(QString contact)
{
	if (CUtils::AreContactsTheSame(contact, m_contact))
	{
		setupUI(1);
	}
}

void DlgCall::OnCallDestroyed()
{
	if (m_bCallInProgress)
	{
		m_bCallInProgress=false;
		setupUI(2);
	}
}

void DlgCall::OnWeHungUp()
{
	m_bCallInProgress=false;
	setupUI(2);
}

bool DlgCall::OnFileOffered(QString contact, int fileSize,
							QString description, QString& dir)
{
	if (!CUtils::AreContactsTheSame(contact, m_contact))
		return false;

	if (QMessageBox::question(
				this, TR("File offered"),
				QString(TR("%1 wants to send you a file.%4"
					"Description: %2 Size: %3.%4"
					"Do you want to accept the incoming file?")).
						arg(CUtils::getUsn(contact)).
						arg(description).
						arg(CUtils::FileSizeToString(fileSize)).
						arg("\n"),
				QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes)
	{
		return false;
	}

	dir = QFileDialog::getExistingDirectory(this, TR("Please select the destination directory"),
								           QDir::home().path(),
										   QFileDialog::ShowDirsOnly
										   | QFileDialog::DontResolveSymlinks);
	bool bTransferAccepted = dir.size() > 0;
	if (bTransferAccepted)
	{
		m_bFileTransferInProgress = true;
	}
	return bTransferAccepted;
}

void DlgCall::btCancelFileTransfer_clicked()
{
	CAllContainer::m_libjingle->cancelFileTransfer();
}

void DlgCall::OnFileTransferProgress(int total, int progress, QString resultDir)
{
	if (m_bFileTransferInProgress == false)
		return;

    if (progress == 0 && total != 0)
	{
		m_pbFileTransfer->setValue(0);
		m_gTopLayout->addWidget(m_fileTransferCtrlsHolder, 2, 0, 2, 0);
		m_ebSendChat->setFocus();
		return;
	}
	if (progress == -1)
	{
		m_bFileTransferInProgress = false;
		m_gTopLayout->removeWidget(m_fileTransferCtrlsHolder);
		QMessageBox::warning(this, TR("File transfer cancelled"),
							 TR("File transfer has been cancelled"));
		m_ebSendChat->setFocus();
		return;
	}
	if (total == progress)
	{
		m_bFileTransferInProgress = false;
		m_pbFileTransfer->setValue(100);
		m_gTopLayout->removeWidget(m_fileTransferCtrlsHolder);

		if (resultDir.isEmpty() == false)
		{
			QString sFinalTxt;
//			sFinalTxt = QString(TR("File(s) have been saved %1here%2")).
//								arg("<A HREF=\"" + resultDir + "/\">").arg("</a>");
			sFinalTxt = QString(TR("File(s) have been saved here: %1")).
								arg(resultDir);
			addChatText(sFinalTxt, "");
			//addChatText("(" + resultDir + ")", "");
		}
		else
		{
			QString sFinalTxt;
			sFinalTxt = QString(TR("Sending the file has finished."));
			addChatText(sFinalTxt, "");
		}

		m_ebSendChat->setFocus();
		return;
	}
	float percent = (float)progress / total;
	m_pbFileTransfer->setTextVisible (true);
	m_pbFileTransfer->setValue((int)(percent*100));

}
