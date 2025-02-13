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

#include "dlgsettings.h"

#include "main.h"
#include "themesmanager.h"
#include "smileysmanager.h"

#include <qapplication.h>
#include <qcombobox.h>
#include <qicon.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qcheckbox.h>

#include <iostream>

#define TR_CONTEXT "SettingsDlg"

DlgSettings::DlgSettings(QWidget *parent)
    : QDialog(parent)
{
	setObjectName("DlgMain");
    setWindowTitle(CAllContainer::m_gtalxVersionNumber + " - " + TR("settings"));
	setWindowIcon(QIcon(CAllContainer::m_themesManager->getActiveThemeDir() + QString("icon_gtalx.png")));

	m_gridLayout = new QGridLayout(this);
    m_gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	m_lbLanguages = new QLabel(this);
	m_lbLanguages->setObjectName(QString::fromUtf8("m_lbLanguages"));
	m_lbLanguages->setText(TR("Selected language:"));
	m_gridLayout->addWidget(m_lbLanguages, 0, 0, 1, 1);

	m_cbLanguages = new QComboBox(this);
    m_cbLanguages->setObjectName(QString::fromUtf8("m_cbLanguages"));
	QStringList installedLanguages;
	int selectedLanguage;
	CAllContainer::m_translator->getInstalledLanguages(installedLanguages, selectedLanguage);
	m_cbLanguages->addItems(installedLanguages);
	m_cbLanguages->setCurrentIndex(selectedLanguage);
    m_gridLayout->addWidget(m_cbLanguages, 0, 1, 1, 2);

	m_lbThemes = new QLabel(this);
	m_lbThemes->setObjectName(QString::fromUtf8("m_lbThemes"));
	m_lbThemes->setText(TR("Selected theme:"));

	m_gridLayout->addWidget(m_lbThemes, 1, 0, 1, 1);

	m_cbThemes = new QComboBox(this);
    m_cbThemes->setObjectName(QString::fromUtf8("m_cbThemes"));
	QStringList installedThemes;
	int selectedTheme;
	CAllContainer::m_themesManager->getInstalledThemes(installedThemes, selectedTheme);
	m_cbThemes->addItems(installedThemes);
	m_cbThemes->setCurrentIndex(selectedTheme);

    m_gridLayout->addWidget(m_cbThemes, 1, 1, 1, 2);

	m_lbSmilies = new QLabel(this);
	m_lbSmilies->setObjectName(QString::fromUtf8("m_lbSmilies"));
	m_lbSmilies->setText(TR("Selected smilies:"));

	m_gridLayout->addWidget(m_lbSmilies, 2, 0, 1, 1);

	m_cbSmilies = new QComboBox(this);
    m_cbSmilies->setObjectName(QString::fromUtf8("m_cbSmilies"));
	QStringList installedSmilies;
	int selectedSmiley;
	CAllContainer::m_smileysManager->getInstalledSmileys(installedSmilies, selectedSmiley);
	m_cbSmilies->addItems(installedSmilies);
	m_cbSmilies->setCurrentIndex(selectedSmiley);

    m_gridLayout->addWidget(m_cbSmilies, 2, 1, 1, 2);
    
    m_cbCloseToSystemTray = new QCheckBox(TR("Close to system tray"), this);
    m_cbCloseToSystemTray->setObjectName("m_cbCloseToSystemTray");
    if (CAllContainer::m_sm["closetosystray"] != "0")
    {
        m_cbCloseToSystemTray->setChecked(true);
    }
    m_gridLayout->addWidget(m_cbCloseToSystemTray, 3, 1, 1, 2);

    m_cbContactNamesInsteadEmails = new QCheckBox(TR("Display contact names instead of emails"), this);
    m_cbContactNamesInsteadEmails->setObjectName("m_cbContactNamesInsteadEmails");
    if (CAllContainer::m_sm["contactNamesInsteadEmails"] != "0")
    {
        m_cbContactNamesInsteadEmails->setChecked(true);
    }
    m_gridLayout->addWidget(m_cbContactNamesInsteadEmails, 4, 1, 1, 2);


    m_lbMicSoundCards = new QLabel(this);
	m_lbMicSoundCards->setObjectName(QString::fromUtf8("m_lbMicSoundCards"));
	m_lbMicSoundCards->setText(TR("Microphone sound card:"));
	m_gridLayout->addWidget(m_lbMicSoundCards, 5, 0, 1, 1);
    
    m_cbMicSoundCards = new QComboBox(this);
    m_cbMicSoundCards->addItem(TR("auto"));
    
    m_gridLayout->addWidget(m_cbMicSoundCards, 5, 1, 1, 2);
    
    m_lbSpeakerSoundCards = new QLabel(this);
	m_lbSpeakerSoundCards->setObjectName(QString::fromUtf8("m_lbSpeakerSoundCards"));
	m_lbSpeakerSoundCards->setText(TR("Speaker sound card:"));
	m_gridLayout->addWidget(m_lbSpeakerSoundCards, 6, 0, 1, 1);
    
    m_cbSpeakerSoundCards = new QComboBox(this);
    m_cbSpeakerSoundCards->addItem(TR("auto"));
    m_gridLayout->addWidget(m_cbSpeakerSoundCards, 6, 1, 1, 2);

    std::vector<std::string> cards;
    CAllContainer::m_libjingle->getSoundcardList(cards);
    for (int i=0; i<cards.size(); ++i)
    {
        m_cbMicSoundCards->addItem(cards[i].c_str());
        m_cbSpeakerSoundCards->addItem(cards[i].c_str());
        
        if (QString(cards[i].c_str()) == CAllContainer::m_sm["micSoundCard"])
        {
            std::cout << "micSoundCard " << cards[i] << std::endl;
            m_cbMicSoundCards->setCurrentIndex(m_cbMicSoundCards->count()-1);
        }
        if (QString(cards[i].c_str()) == CAllContainer::m_sm["speakerSoundCard"])
        {
            std::cout << "speakerSoundCard " << cards[i] << std::endl;
            m_cbSpeakerSoundCards->setCurrentIndex(m_cbSpeakerSoundCards->count()-1);
        }
    }

	m_btSave = new QPushButton(this);
	m_btSave->setObjectName("m_btSave");
	m_btSave->setText(TR("Save"));
	m_gridLayout->addWidget(m_btSave, 7, 0, 1, 1);

	m_btRestoreDefaults = new QPushButton(this);
	m_btRestoreDefaults->setObjectName("m_btRestoreDefaults");
	m_btRestoreDefaults->setText(TR("Restore defaults"));
	m_gridLayout->addWidget(m_btRestoreDefaults, 7, 1, 1, 1);

	m_btCancel = new QPushButton(this);
	m_btCancel->setObjectName("m_btCancel");
	m_btCancel->setText(TR("Cancel"));
	m_gridLayout->addWidget(m_btCancel, 7, 2, 1, 1);

	connect( m_btSave, SIGNAL( clicked() ), this, SLOT( m_btSave_clicked() ) );
	connect( m_btRestoreDefaults, SIGNAL( clicked() ), this, SLOT( m_btRestoreDefaults_clicked() ) );
	connect( m_btCancel, SIGNAL( clicked() ), this, SLOT( m_btCancel_clicked() ) );

	resize(300, 200);
	CAllContainer::m_themesManager->applyStyleToWidget("settingsdlg.css", this);
}

DlgSettings::~DlgSettings()
{
}

void DlgSettings::m_btSave_clicked()
{
	CAllContainer::m_sm.SetSetting("ActiveTheme", m_cbThemes->currentText());
	CAllContainer::m_sm.SetSetting("SmileyIcons", m_cbSmilies->currentText());
	if (m_cbLanguages->currentText() != CAllContainer::m_translator->getCurrentLanguage())
	{
		CAllContainer::m_sm.SetSetting("language", m_cbLanguages->currentText());
	}

    if (m_cbCloseToSystemTray->isChecked())
        CAllContainer::m_sm.SetSetting("closetosystray", "1");
    else
        CAllContainer::m_sm.SetSetting("closetosystray", "0");

    if (m_cbContactNamesInsteadEmails->isChecked())
        CAllContainer::m_sm.SetSetting("contactNamesInsteadEmails", "1");
    else
        CAllContainer::m_sm.SetSetting("contactNamesInsteadEmails", "0");

    if (m_cbMicSoundCards->currentText() == TR("auto"))
    {
        CAllContainer::m_sm.SetSetting("micSoundCard", "");
    }
    else
    {
        CAllContainer::m_sm.SetSetting("micSoundCard", m_cbMicSoundCards->currentText());
    }
    
    if (m_cbSpeakerSoundCards->currentText() == TR("auto"))
    {
        CAllContainer::m_sm.SetSetting("speakerSoundCard", "");
    }
    else
    {
        CAllContainer::m_sm.SetSetting("speakerSoundCard", m_cbSpeakerSoundCards->currentText());
    }
    
    CAllContainer::m_sm.saveSettings();
	QMessageBox::information(this, "", TR("Settings saved, please restart gtalX"));
	close();
}

void DlgSettings::m_btRestoreDefaults_clicked()
{
	CAllContainer::m_sm.SetSetting("ActiveTheme", "default");
	CAllContainer::m_sm.SetSetting("SmileyIcons", "default");
	CAllContainer::m_sm.SetSetting("language", "");
    CAllContainer::m_sm.SetSetting("closetosystray", "");
    CAllContainer::m_sm.SetSetting("contactNamesInsteadEmails", "");
    CAllContainer::m_sm.SetSetting("micSoundCard", "");
    CAllContainer::m_sm.SetSetting("speakerSoundCard", "");

    CAllContainer::m_sm.saveSettings();

	QMessageBox::information(this, "", TR("Settings saved, please restart gtalX"));
	close();
}

void DlgSettings::m_btCancel_clicked()
{
	close();
}
