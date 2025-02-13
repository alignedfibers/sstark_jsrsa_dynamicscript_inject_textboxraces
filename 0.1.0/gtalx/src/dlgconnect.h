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

#ifndef DLGCONNECT_H
#define DLGCONNECT_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgridlayout.h>
#include <qcursor.h>

class DlgConnect : public QDialog
{
    Q_OBJECT

 public:
    DlgConnect(QWidget *parent = 0);

 protected slots:
    void btConnect_clicked();
	void OnNewStatus(int statusId, QString status);
	void OnError(int errId, QString error); 

protected:
    QGridLayout* m_gridLayout;

    QPushButton* m_btConnect;
    QLabel* m_lbEmail;
    QLabel* m_lbPassword;
    QLabel* m_lbImage;
    QLineEdit* m_ebEmail;
    QLineEdit* m_ebPassword;
};


#endif
