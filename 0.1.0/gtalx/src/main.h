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

#ifndef MAIN_H_
#define MAIN_H_

#include <qstring.h>

#include <vector>
#include <map>

#include "settingsmanager.h"
#include "translator.h"

class CLibjingleIf;
class CJingleCallbackImpl;
class QWidget;
class CCallDlgManager;
class CContactsManager;
class CSmileysManager;
class CTrayIconManager;
class CThemesManager;

class CAllContainer
{
public:
    static CJingleCallbackImpl* m_cbimpl;
    static CLibjingleIf* m_libjingle;
	static CCallDlgManager* m_callDlgManager;
	static CContactsManager* m_contactsManager;
	static CSmileysManager* m_smileysManager;
    static CSettingsManager m_sm;
    static CTranslator* m_translator;
	static CTrayIconManager* m_trayIconManager;
	static CThemesManager* m_themesManager;

    static QString m_email;
    static QString m_password;

    static QWidget* m_mainWnd;
	static QString m_sharePath;
	static QString m_gtalxVersionNumber;
};

#define TR(p) \
    CAllContainer::m_translator->translate(TR_CONTEXT, p)


#endif
