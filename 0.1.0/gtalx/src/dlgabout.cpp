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

#include "dlgabout.h"

#include "main.h"
#include "jinglecallbackimpl.h"
#include "utils.h"
#include "themesmanager.h"

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtextbrowser.h>

#define TR_CONTEXT "AboutDlg"

DlgAbout::DlgAbout(QWidget *parent)
    : QDialog(parent)
{
    setObjectName(QString::fromUtf8("DlgAbout"));
 	setWindowIcon(QIcon(CAllContainer::m_sharePath + QString("icon_aboutdlg.png")));
	setWindowTitle(CAllContainer::m_gtalxVersionNumber + " - " + TR("about"));

    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setObjectName(QString::fromUtf8("m_gridLayout"));

    m_lbImage = new QLabel(this);
    m_lbImage->setObjectName(QString::fromUtf8("m_lbImage"));
	QPixmap pm(CAllContainer::m_sharePath + QString("pic_aboutdlg.png"));
	m_lbImage->setPixmap(pm.scaled(351, 200,Qt::KeepAspectRatioByExpanding));
	m_lbImage->setScaledContents(true);

    m_gridLayout->addWidget(m_lbImage, 0, 0, 1, 1);

    m_ebText = new QTextBrowser(this);
	m_ebText->setObjectName(QString::fromUtf8("m_ebText"));
	m_ebText->setTextInteractionFlags(Qt::TextSelectableByMouse |
									  Qt::LinksAccessibleByMouse |
									  Qt::LinksAccessibleByKeyboard);
	m_ebText->setOpenExternalLinks(true);
	QString sTxt;
	sTxt += QString(TR("<i>%1</i> is a Gtalk client using Google's libjingle library.")).arg(CAllContainer::m_gtalxVersionNumber);
	sTxt += "<p>";
	sTxt += QString(TR("Check out the %1homepage%2 of <i>gtalX</i>!!!")).arg(
					"<A HREF=\"http://sites.google.com/site/jozsefbekes/Home/gtalx\">").arg(
					"</a>");
	sTxt += "<p>";

	QString sTranslatedBy(CAllContainer::m_translator->translate(TR_CONTEXT, "Translated by"));
	if (sTranslatedBy != "Translated by")
	{
		sTxt += sTranslatedBy;
		sTxt += "<p>";
	}
	QString sThemeAuthor(CAllContainer::m_themesManager->getThemeAuthor());
	if (sThemeAuthor != "")
	{
		sTxt += QString(TR("Theme created by: %1")).arg(sThemeAuthor);
		sTxt += "<p>";
	}

	sTxt += "<i>gtalX</i> is free software: you can redistribute it and/or modify "
			"it under the terms of the GNU General Public License as published by "
			"the Free Software Foundation, either version 3 of the License, or "
			"(at your option) any later version."
			"<p>"
			"This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; "
			"without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
			"See the GNU General Public License for more details ("
			"<A HREF=\"http://www.gnu.org/licenses\">http://www.gnu.org/licenses/</a>).";

    m_ebText->setText(sTxt);
	//m_ebText->append("<A HREF=\"http://sites.google.com/site/jozsefbekes/Home/gtalx\">aaa</a>");

	m_gridLayout->addWidget(m_ebText, 1, 0, 1, 1);

    m_btOk = new QPushButton(this);
    m_btOk->setObjectName(QString::fromUtf8("m_btOk"));
    m_btOk->setText(TR("OK"));
    m_gridLayout->addWidget(m_btOk, 2, 0, 1, 1);

    CUtils::setWidgetBackground(this, "aboutDlgBColor", "white");

    connect( m_btOk, SIGNAL( clicked() ), this, SLOT( btOk_clicked() ) );
}

DlgAbout::~DlgAbout()
{
    delete m_gridLayout;
}

void DlgAbout::btOk_clicked()
{
    done(1);
}
