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

#ifndef __ccalldlgmanager__
#define __ccalldlgmanager__

#include <map>
#include <vector>

#include <qstring.h>
#include <qobject.h>
#include <qpoint.h>

#include "dlgcall.h"

class DlgCall;

class CCallDlgManager : public QObject
{
	Q_OBJECT

friend class DlgCall;

public:
	CCallDlgManager();
	virtual ~CCallDlgManager();
	
	void RegisterDlg(DlgCall* dlg);
	void UnregisterDlg(DlgCall* dlg);
	
	DlgCall* DisplayDlg(QString contact, QWidget* parent, DlgCall::eReason reason);
	
	void SetAllDlgsVisible(bool visible);
	void NotifyAllDlgsWeHungUp();
protected slots:
	void OnIncomingChat(QString iconset,
                        QString contact,
                        QString text);
	void OnIncomingCall(QString contact);
	void OnFileReceiveStart(QString sender, int fileSize, QString description, long ftsd);

protected:
    void SetupNewDlg(DlgCall* dlgCall);
	void SetDlgPos(DlgCall* dlgCall);
	
	std::vector<DlgCall*> m_dlgs;
	bool m_callDlgsVisible;
	
	QPoint m_initialPos;
};

#endif // __ccalldlgmanager__
