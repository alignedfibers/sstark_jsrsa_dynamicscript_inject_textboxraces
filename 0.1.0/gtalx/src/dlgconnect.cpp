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

#include "dlgconnect.h"
#include "jinglecallbackimpl.h"
#include "utils.h"
#include "main.h"
#include "themesmanager.h"

#include <qmessagebox.h>
#include <qapplication.h>

#define TR_CONTEXT "ConnectDlg"

DlgConnect::DlgConnect(QWidget *parent)
    : QDialog(parent)
{

    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("DlgConnectGen"));

	setWindowTitle(CAllContainer::m_gtalxVersionNumber);
    setWindowModality(Qt::WindowModal);
    resize(369, 312);
    setBaseSize(QSize(0, 0));
    setWindowIcon(QIcon(CAllContainer::m_themesManager->getActiveThemeDir() + QString("icon_gtalx.png")));

    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    m_lbEmail = new QLabel(this);
    m_lbEmail->setObjectName(QString::fromUtf8("m_lbEmail"));
    m_lbEmail->setText(TR("Email"));
    m_gridLayout->addWidget(m_lbEmail, 1, 0, 1, 1);

    m_ebEmail = new QLineEdit(this);
    m_ebEmail->setObjectName(QString::fromUtf8("m_ebEmail"));
    m_gridLayout->addWidget(m_ebEmail, 1, 1, 1, 1);

    m_lbPassword = new QLabel(this);
    m_lbPassword->setObjectName(QString::fromUtf8("m_lbPassword"));
    m_lbPassword->setText(TR("Password"));
    m_gridLayout->addWidget(m_lbPassword, 2, 0, 1, 1);

    m_ebPassword = new QLineEdit(this);
    m_ebPassword->setObjectName(QString::fromUtf8("m_ebPassword"));
    m_ebPassword->setEchoMode(QLineEdit::Password);
    m_gridLayout->addWidget(m_ebPassword, 2, 1, 1, 1);

    m_btConnect = new QPushButton(this);
    m_btConnect->setObjectName(QString::fromUtf8("m_btConnect"));
    m_btConnect->setText(TR("Connect"));

    m_gridLayout->addWidget(m_btConnect, 3, 0, 1, 2);

    m_lbImage = new QLabel(this);
    m_lbImage->setObjectName(QString::fromUtf8("m_lbImage"));
    m_lbImage->setMinimumSize(QSize(351, 200));
    m_lbImage->setBaseSize(QSize(351, 200));
    m_lbImage->setPixmap(QPixmap(CAllContainer::m_themesManager->getActiveThemeDir() + QString("pic_connectdlg.png")));
    m_lbImage->setScaledContents(true);
    m_gridLayout->addWidget(m_lbImage, 0, 0, 1, 2);

    m_ebEmail->setText(CAllContainer::m_sm["lastEmail"]);

    connect( m_btConnect, SIGNAL( clicked() ), this, SLOT( btConnect_clicked() ) );
	connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnNewStatus(int, QString)),
            this, SLOT(OnNewStatus(int, QString)));
	connect(&CAllContainer::m_cbimpl->m_QTCB, SIGNAL(OnError(int, QString)),
            this, SLOT(OnError(int, QString)));

	CAllContainer::m_themesManager->applyStyleToWidget("connectdlg.css", this);
}

void DlgConnect::btConnect_clicked()
{
	CAllContainer::m_email = m_ebEmail->text();
	CAllContainer::m_password = m_ebPassword->text();

	CAllContainer::m_libjingle->connect(
			CAllContainer::m_email.toStdString(),
			CAllContainer::m_password.toStdString());

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	m_btConnect->setEnabled(false);
    m_ebEmail->setEnabled(false);
    m_ebPassword->setEnabled(false);
}

void DlgConnect::OnNewStatus(int statusId, QString status)
{
	if (statusId == CJingleCallback::eLoggedIn)
	{
		QApplication::restoreOverrideCursor();
		done(1);
	}
}

void DlgConnect::OnError(int errId, QString error)
{
	QApplication::restoreOverrideCursor();
	QMessageBox::warning(this, TR("Error"), error);

	m_btConnect->setEnabled(true);
    m_ebEmail->setEnabled(true);
    m_ebPassword->setEnabled(true);
}
