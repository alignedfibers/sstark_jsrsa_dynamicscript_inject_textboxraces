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

#ifndef DLGMAIN_H_
#define DLGMAIN_H_

#include "contactswidget.h"
#include "interface/libjingleif.h"

#include <qdialog.h>

class DlgMain : public QWidget
{
	Q_OBJECT

 public:
    DlgMain(QWidget *parent = 0);
    virtual ~DlgMain();

	void close();

    QLabel* m_IdLoggedIn;
    QLabel* m_StatusBar;

protected slots:
    void btAbout_clicked();
	void btSettings_clicked();

    void OnNewStatus(int statusId, QString status);
	void OnContactStatus(QString contact, QString name, QString status, int capabilities);
	void OnError(int errId, QString error);

	void OnCallDestroyed();

protected:
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);

	CContactsWidget* m_ContactsCtrl;
	bool m_bDoCLose;

};

#endif
