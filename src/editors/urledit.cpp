/* This file is part of the KDE project
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>
   Copyright (C) 2004  Alexander Dymo <cloudtemple@mskat.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "urledit.h"

#include <qlayout.h>
#include <qvariant.h>

#ifndef QT_ONLY
#include <kurlrequester.h>
#else
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#endif

#include "property.h"


namespace KOProperty {

URLEdit::URLEdit(Property *property, QWidget *parent, const char *name)
 : Widget(property, parent, name)
{
	QHBoxLayout *l = new QHBoxLayout(this, 0, 0);
#ifndef QT_ONLY
	m_edit = new KURLRequester(this);
	m_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_edit->setMinimumHeight(5);
	l->addWidget(m_edit);

	setProperty(property);

	connect(m_edit, SIGNAL(textChanged(const QString&)), this, SLOT(slotValueChanged(const QString&)));
#else
	m_edit = new QLineEdit(this);
	m_select = new QPushButton("...",this);
	l->addWidget(m_edit);
	l->addWidget(m_select);
	connect( m_select, SIGNAL(clicked()),this,SLOT(selectFile()));
	connect(m_edit, SLOT(textChanged(const QString&)), this, slotValueChanged(const QString&));
#endif
	m_edit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

URLEdit::~URLEdit()
{}

QVariant
URLEdit::value() const
{
#ifdef QT_ONLY
	return m_edit->text();
#else
	return m_edit->url();
#endif
}

void
URLEdit::setValue(const QVariant &value, bool emitChange)
{
	m_edit->blockSignals(true);
#ifdef QT_ONLY
	m_edit->setText(value.toString());
#else
	m_edit->setURL(value.toString());
#endif
	m_edit->blockSignals(false);
	if (emitChange)
		emit valueChanged(this);
}

void
URLEdit::slotValueChanged(const QString&)
{
	emit valueChanged(this);
}

void
URLEdit::setProperty(Property *property)
{
#ifndef QT_ONLY
	int  mode;
	if(property) {
		switch(property->type()) {
			case DirectoryURL:  mode = KFile::Directory|KFile::ExistingOnly;  break;
			case FileURL: case PictureFileURL: default: mode = KFile::File|KFile::ExistingOnly;
		}
		m_edit->setMode(mode);
	}
#endif

	Widget::setProperty(property);
}

void
URLEdit::selectFile()
{
#ifdef QT_ONLY
	QString path;
	if(m_property->type() == DirectoryURL)
		path = QFileDialog::getExistingDirectory(path,this);
	else if(m_property->type() == FileURL)
		path = QFileDialog::getOpenFileName(path, QString::null, this);
	//slotValueChanged(m_url);
	m_edit->setText(path);
#endif
}

}

#include "urledit.moc"
