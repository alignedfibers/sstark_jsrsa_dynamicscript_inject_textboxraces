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

#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <qdialog.h>

class QHBoxLayout;
class QVBoxLayout;
class CBEvent;
class QTextBrowser;
class QGridLayout;
class QPushButton;
class QLabel;

class DlgAbout : public QDialog
{
    Q_OBJECT
public:
    DlgAbout(QWidget* parent);
    virtual ~DlgAbout();

protected slots:	
    void btOk_clicked();

protected:
    QLabel* m_lbImage;
	QTextBrowser* m_ebText;
    QGridLayout* m_gridLayout;
    QPushButton* m_btOk;
};

#endif
