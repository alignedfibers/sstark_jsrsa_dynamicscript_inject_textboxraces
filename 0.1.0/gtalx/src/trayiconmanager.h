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

#ifndef __ctrayiconmanager__
#define __ctrayiconmanager__

#include <qobject.h>
#include <qicon.h>
#include <qsystemtrayicon.h>

class QMenu;
class QWidget;
class QSystemTrayIcon;
class QAction;

class DlgMain;

class CTrayIconManager : public QObject
{
	Q_OBJECT

public:
	CTrayIconManager(DlgMain* mainWnd);
	virtual ~CTrayIconManager();

    void ShowTrayMessage(QString title, QString message);
    bool isTrayIconAvailable() const;

protected slots:
    void quit();
	void activated(QSystemTrayIcon::ActivationReason reason);
	void messageClicked();

protected:
    void createActions();

	QIcon m_icon;
    DlgMain* m_mainWnd;
	QMenu* m_trayIconMenu;
	QSystemTrayIcon* m_trayIcon;

	QAction* m_quitAction;
};
#endif // __ctrayiconmanager__
