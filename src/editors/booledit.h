/* This file is part of the KDE project
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>
   Copyright (C) 2004 Alexander Dymo <cloudtemple@mskat.net>
   Copyright (C) 2006-2015 Jarosław Staniek <staniek@kde.org>

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

#ifndef KPROPERTY_BOOLEDIT_H
#define KPROPERTY_BOOLEDIT_H

#include "combobox.h"
#include "KPropertyFactory.h"

#include <QEvent>
#include <QResizeEvent>
#include <QToolButton>

//! A bool editor supporting two states: true and false.
/*! For null values, false is displayed.
*/
class KPROPERTYWIDGETS_EXPORT KPropertyBoolEditor : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue USER true)

public:
    explicit KPropertyBoolEditor(const KProperty *prop, QWidget *parent = nullptr);

    ~KPropertyBoolEditor() override;

    QVariant value() const;

    static void draw(QPainter *p, const QRect &r, const QVariant &value,
                     const QString& text, bool threeState);
Q_SIGNALS:
    void commitData(QWidget* editor);

public Q_SLOTS:
    void setValue(const QVariant &value);

protected Q_SLOTS:
    void  slotValueChanged(bool state);

protected:
    void paintEvent( QPaintEvent * event ) override;
    bool eventFilter(QObject* watched, QEvent* e) override;

private:
    Q_DISABLE_COPY(KPropertyBoolEditor)
    class Private;
    Private * const d;
};

//! A bool editor supporting three states: true, false and null.
/*! The editor is implemented as a drop-down list.
*/
class KPROPERTYWIDGETS_EXPORT KPropertyThreeStateBoolEditor : public KPropertyComboBoxEditor
{
    Q_OBJECT

public:
    explicit KPropertyThreeStateBoolEditor(const KPropertyListData& listData, QWidget *parent = nullptr);
    ~KPropertyThreeStateBoolEditor() override;

    QVariant value() const override;
    void setValue(const QVariant &value) override;

private:
    Q_DISABLE_COPY(KPropertyThreeStateBoolEditor)
    class Private;
    Private * const d;
};

class KPROPERTYWIDGETS_EXPORT KPropertyBoolDelegate : public KPropertyEditorCreatorInterface,
                                                      public KPropertyValuePainterInterface,
                                                      public KPropertyValueDisplayInterface
{
public:
    KPropertyBoolDelegate();

    QWidget *createEditor(int type, QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QString propertyValueToString(const KProperty *prop, const QLocale &locale) const override;

    QString valueToString(const QVariant &value, const QLocale &locale) const override;
};

#endif
