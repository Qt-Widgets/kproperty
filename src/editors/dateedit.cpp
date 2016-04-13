/* This file is part of the KDE project
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>
   Copyright (C) 2004 Alexander Dymo <cloudtemple@mskat.net>
   Copyright (C) 2012 Friedrich W. H. Kossebau <kossebau@kde.org>
   Copyright (C) 2015 Jarosław Staniek <staniek@kde.org>

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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#include "dateedit.h"

#include "KPropertyEditorDataModel.h"

#include <QLocale>

KPropertyDateEditor::KPropertyDateEditor(const KProperty* prop, QWidget* parent)
  : QDateEdit(parent)
{
    setFrame(false);
    setCalendarPopup(true);

    const QDate minDate = prop->option("min").toDate();
    if (minDate.isValid()) {
        setMinimumDate(minDate);
    }
    const QDate maxDate = prop->option("max").toDate();
    if (maxDate.isValid()) {
        setMaximumDate(maxDate);
    }

    connect(this, SIGNAL(dateChanged(QDate)), this, SLOT(onDateChanged()));
}

KPropertyDateEditor::~KPropertyDateEditor()
{
}

QVariant KPropertyDateEditor::value() const
{
    return QVariant(date());
}

void KPropertyDateEditor::setValue(const QVariant& value)
{
    blockSignals(true);
    setDate(value.toDate());
    blockSignals(false);
}

void KPropertyDateEditor::paintEvent(QPaintEvent* event)
{
    QDateEdit::paintEvent(event);
    KPropertyWidgetsFactory::paintTopGridLine(this);
}


void KPropertyDateEditor::onDateChanged()
{
    emit commitData(this);
}


//! @todo Port to KLocale, be inspired by KexiDateTableEdit (with Kexi*Formatter)
KPropertyDateDelegate::KPropertyDateDelegate()
{
}

QString KPropertyDateDelegate::valueToString(const QVariant& value, const QLocale &locale) const
{
    const QString defaultDateFormat = locale.dateFormat(QLocale::ShortFormat);
    return value.toDate().toString(defaultDateFormat);
}

QWidget* KPropertyDateDelegate::createEditor(int type, QWidget* parent,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(type);
    Q_UNUSED(option);

    const KPropertyEditorDataModel *editorModel
        = dynamic_cast<const KPropertyEditorDataModel*>(index.model());
    KProperty *prop = editorModel ? editorModel->propertyForItem(index) : 0;
    if (!prop) {
        return 0;
    }
    return new KPropertyDateEditor(prop, parent);
}
