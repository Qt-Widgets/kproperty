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

#include "booledit.h"

#ifndef QT_ONLY
#include <kiconloader.h>
#include <klocale.h>
#endif

#include <qtoolbutton.h>
#include <qpainter.h>
#include <qvariant.h>
#include <qlayout.h>

#include <kdebug.h>

using namespace KOProperty;

BoolEdit::BoolEdit(Property *property, QWidget *parent, const char *name)
 : Widget(property, parent, name)
{
    m_toggle = new QToolButton(this);
    m_toggle->setFocusPolicy(QWidget::NoFocus);
    m_toggle->setToggleButton(true);
    m_toggle->setUsesTextLabel(true);
    m_toggle->setTextPosition(QToolButton::Right);
    m_toggle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //we're not using layout to because of problems with button size
    m_toggle->move(0, 0);
    m_toggle->resize(width(), height());
    m_toggle->show();

    setFocusWidget(m_toggle);
    connect(m_toggle, SIGNAL(toggled(bool)), this, SLOT(slotValueChanged(bool)));
}

BoolEdit::~BoolEdit()
{}

QVariant
BoolEdit::value() const
{
    return QVariant(m_toggle->isOn(), 4);
}

void
BoolEdit::setValue(const QVariant &value, bool emitChange)
{
    m_toggle->blockSignals(true);
    m_toggle->setOn(value.toBool());
    setState(value.toBool());
    m_toggle->blockSignals(false);
    if (emitChange)
        emit valueChanged(this);
}

void
BoolEdit::slotValueChanged(bool state)
{
    setState(state);
    emit valueChanged(this);
}

void
BoolEdit::drawViewer(QPainter *p, const QColorGroup &, const QRect &r, const QVariant &value)
{
    p->eraseRect(r);
    QRect r2(r);
    r2.moveLeft(KIcon::SizeSmall + 6);

    if(value.toBool()) {
        p->drawPixmap(3, (r.height()-1-KIcon::SizeSmall)/2, SmallIcon("button_ok"));
        p->drawText(r2, Qt::AlignVCenter | Qt::AlignLeft, i18n("True"));
    }
    else  {
        p->drawPixmap(3, (r.height()-1-KIcon::SizeSmall)/2, SmallIcon("button_cancel"));
        p->drawText(r2, Qt::AlignVCenter | Qt::AlignLeft, i18n("False"));
    }
}

void
BoolEdit::setState(bool state)
{
    if(state)
    {
        m_toggle->setIconSet(QIconSet(SmallIcon("button_ok")));
        m_toggle->setTextLabel(i18n("True"));
    }
    else
    {
        m_toggle->setIconSet(QIconSet(SmallIcon("button_cancel")));
        m_toggle->setTextLabel(i18n("False"));
    }
}

void
BoolEdit::resizeEvent(QResizeEvent *ev)
{
    m_toggle->resize(ev->size());
}

bool
BoolEdit::eventFilter(QObject* watched, QEvent* e)
{
    if(e->type() == QEvent::KeyPress) {
        QKeyEvent* ev = static_cast<QKeyEvent*>(e);
        if(ev->key() == Key_Space) {
            m_toggle->toggle();
            return true;
        }
    }
    return Widget::eventFilter(watched, e);
}

#include "booledit.moc"
