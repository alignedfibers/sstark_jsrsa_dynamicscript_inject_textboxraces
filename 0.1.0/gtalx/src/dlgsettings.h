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

#ifndef __dlgsettings__
#define __dlgsettings__

#include "interface/libjingleif.h"

#include <qdialog.h>

class QGridLayout;
class QComboBox;
class QLabel;
class QPushButton;
class QCheckBox;

class DlgSettings : public QDialog
{
	Q_OBJECT

public:
	DlgSettings(QWidget *parent = 0);
	virtual ~DlgSettings();

protected slots:
    void m_btSave_clicked();
	void m_btCancel_clicked();
	void m_btRestoreDefaults_clicked();

protected:
    QGridLayout* m_gridLayout;

	QLabel* m_lbLanguages;
	QComboBox* m_cbLanguages;
	QLabel* m_lbThemes;
	QComboBox* m_cbThemes;
	QLabel* m_lbSmilies;
	QComboBox* m_cbSmilies;
    QCheckBox* m_cbCloseToSystemTray;
    QCheckBox* m_cbContactNamesInsteadEmails;
    QLabel* m_lbMicSoundCards;
    QComboBox* m_cbMicSoundCards;
    QLabel* m_lbSpeakerSoundCards;
    QComboBox* m_cbSpeakerSoundCards;

	QPushButton* m_btSave;
	QPushButton* m_btCancel;
	QPushButton* m_btRestoreDefaults;

};

#endif // __dlgsettings__
