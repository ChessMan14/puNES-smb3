/*
 *  Copyright (C) 2010-2023 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <QtCore/QDateTime>
#include <QtCore/QEvent>
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include "dlgAbout.hpp"
#include "version.h"
#include "info.h"

dlgAbout::dlgAbout(QWidget *parent) : QDialog(parent) {
	QDateTime compiled = QDateTime::fromString(COMPILED, "MMddyyyyhhmmss");
	QString text;

	setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);

	if (font().pointSize() > 9) {
		QFont font;

		font.setPointSize(9);
		setFont(font);
	}

	setWindowTitle(QString(NAME));
	setWindowModality(Qt::WindowModal);

	setWindowIcon(QIcon(":/icon/icons/application.png"));
	icon_About->setPixmap(QPixmap(":/pics/pics/punes_banner.png"));

	text.append("<center><h2>" + QString(NAME) + " ");
	if (info.portable) {
		text.append(tr("portable version") + " ");
	}
	text.append(QString(VERSION) + "</h2></center>");

#if defined (WITH_GIT_INFO)
	text.append("<center><h4>[<font color='#800000'>Commit " + QString(GIT_COUNT_COMMITS) + "</font> " + "<a href=\"https://github.com/punesemu/puNES/commit/" + QString(GIT_LAST_COMMIT_HASH) + "\">" + QString(GIT_LAST_COMMIT) + "</a>]</h4></center>");
#endif
	text.append("<center>" + tr("Nintendo Entertainment System Emulator") + "</center>");
	text.append("<center>" + tr("Compiled") + " " + QLocale().toString(compiled, QLocale::ShortFormat) + " (" + QString(ENVIRONMENT));
	text.append(", " + QString(VERTYPE) + ")</center>");
	label_Text->setText(text);

	text = "<center>" + QString(COPYRUTF8) + "</center>\n";
	text.append("<center><a href=\"" + QString(GITLAB) + "\">" + "GitLab Page</a></center>");
	text.append("<center><a href=\"" + QString(GITHUB) + "\">" + "GitHub Page</a></center>");
	text.append("<center><a href=\"" + QString(WEBSITE) + "\">" + "NesDev Forum</a></center>");
	label_Informative->setText(text);

	connect(pushButton_Ok, SIGNAL(clicked(bool)), this, SLOT(s_ok_clicked(bool)));
}
dlgAbout::~dlgAbout() = default;

void dlgAbout::changeEvent(QEvent *event) {
	if (event->type() == QEvent::LanguageChange) {
		retranslateUi(this);
	} else {
		QDialog::changeEvent(event);
	}
}

void dlgAbout::s_ok_clicked(UNUSED(bool checked)) {
	close();
}
