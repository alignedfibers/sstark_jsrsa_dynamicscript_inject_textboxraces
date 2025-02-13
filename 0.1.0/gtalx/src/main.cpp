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

#include <qapplication.h>
#include "dlgmain.h"
#include "dlgconnect.h"
#include "dlgcall.h"
#include "utils.h"
#include "calldlgmanager.h"
#include "contactsmanager.h"
#include "main.h"
#include "jinglecallbackimpl.h"
#include "smileysmanager.h"
#include "trayiconmanager.h"
#include "themesmanager.h"

CJingleCallbackImpl* CAllContainer::m_cbimpl = NULL;
CLibjingleIf* CAllContainer::m_libjingle = NULL;
CCallDlgManager* CAllContainer::m_callDlgManager = NULL;
CContactsManager* CAllContainer::m_contactsManager = NULL;
QString CAllContainer::m_email;
QString CAllContainer::m_password;
CSettingsManager CAllContainer::m_sm("settings.txt");
QWidget* CAllContainer::m_mainWnd = NULL;
CTranslator* CAllContainer::m_translator = NULL;
QString CAllContainer::m_sharePath("../share/gtalx/");
CSmileysManager* CAllContainer::m_smileysManager = NULL;
CTrayIconManager* CAllContainer::m_trayIconManager = NULL;
CThemesManager* CAllContainer::m_themesManager = NULL;
QString CAllContainer::m_gtalxVersionNumber("gtalX 0.1.0");

int main( int argc, char ** argv )
{
	CUtils::log("You can get verbose logs if you start gtalx with -d");

    bool debug = false;
    if (argc > 1 && !strcmp(argv[1], "-d"))
      debug = true;

    QApplication a( argc, argv );

    if (QApplication::applicationDirPath().right(6) == "/.libs")
		CAllContainer::m_sharePath = QApplication::applicationDirPath() + "/../../share/gtalx/";
	else
		CAllContainer::m_sharePath = QApplication::applicationDirPath() + "/../share/gtalx/";

	CUtils::log("Share files directory: " + CAllContainer::m_sharePath);

	CAllContainer::m_themesManager = new CThemesManager();
	CAllContainer::m_translator = new CTranslator();
	CAllContainer::m_smileysManager = new CSmileysManager();
    CAllContainer::m_cbimpl = new CJingleCallbackImpl();
	CAllContainer::m_contactsManager = new CContactsManager();
    CAllContainer::m_libjingle = CLibjingleIf::create(
    								CAllContainer::m_cbimpl,
									CAllContainer::m_sm["GtalkServer"].toStdString(),
									CAllContainer::m_sm["GtalkPort"].toInt(),
    								QString(CAllContainer::m_themesManager->getActiveThemeDir() + "incomingring.wav").toStdString(),
    								QString(CAllContainer::m_themesManager->getActiveThemeDir() + "outgoingring.wav").toStdString(),
									QString(CAllContainer::m_themesManager->getActiveThemeDir() + "chatding.wav").toStdString(),
									debug,
									CAllContainer::m_sm["ProxyHost"].toStdString(),
									CAllContainer::m_sm["ProxyPort"].toInt());
                                    
    CAllContainer::m_libjingle->selectSoundCards(CAllContainer::m_sm["micSoundCard"].toStdString(), 
                                                 CAllContainer::m_sm["speakerSoundCard"].toStdString());

	CAllContainer::m_callDlgManager = new CCallDlgManager();

	CAllContainer::m_mainWnd = new DlgMain();

	DlgConnect connWnd;
    if (connWnd.exec() == QDialog::Rejected)
    {
		delete CAllContainer::m_callDlgManager;
		CAllContainer::m_callDlgManager = NULL;
		delete CAllContainer::m_mainWnd;
		CAllContainer::m_mainWnd = NULL;
		delete CAllContainer::m_translator;
		CAllContainer::m_translator = NULL;
		CUtils::log("Bye.");
    	return 0;
	}

    CAllContainer::m_sm.SetSetting("lastEmail", CAllContainer::m_email);

	CAllContainer::m_trayIconManager = new CTrayIconManager((DlgMain*)CAllContainer::m_mainWnd);

	CAllContainer::m_mainWnd->show();

	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	int ret = a.exec();

	delete CAllContainer::m_trayIconManager;
	delete CAllContainer::m_callDlgManager;
	CAllContainer::m_callDlgManager = NULL;
	delete CAllContainer::m_mainWnd;
	CAllContainer::m_mainWnd = NULL;

    delete CAllContainer::m_libjingle;
	delete CAllContainer::m_contactsManager;
	delete CAllContainer::m_smileysManager;
    delete CAllContainer::m_cbimpl;
	delete CAllContainer::m_translator;
	CAllContainer::m_translator = NULL;

	if (CAllContainer::m_sm["gtalx_version"] == "")
	    CAllContainer::m_sm.SetSetting("gtalx_version", "0.0.5.0");

	CAllContainer::m_sm.saveSettings();

	CUtils::log("Bye.");
    return ret;
}

