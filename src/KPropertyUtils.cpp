/* This file is part of the KDE project
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>
   Copyright (C) 2004 Alexander Dymo <cloudtemple@mskat.net>
   Copyright (C) 2004-2017 Jarosław Staniek <staniek@kde.org>

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

#include "KPropertyUtils.h"
#include "KPropertyUtils_p.h"
#include "KPropertyEditorDataModel_p.h"
#include "KPropertyEditorView.h"

#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QStyleOption>
#include <QStyleOptionHeader>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPointer>
#include <QMetaEnum>

#define BRANCHBOX_SIZE 9

//! @internal
static void paintListViewExpander(QPainter* p, QWidget* w, int height, const QPalette& palette, bool isOpen)
{
    const int marg = (height - 2 - BRANCHBOX_SIZE) / 2;
    int xmarg = marg;
#if 0
//! @todo disabled: kstyles do not paint background yet... reenable in the future...
    KStyle* kstyle = dynamic_cast<KStyle*>(widget->style());
    if (kstyle) {
        kstyle->drawKStylePrimitive(
            KStyle::KPE_ListViewExpander, p, w, QRect(xmarg, marg, BRANCHBOX_SIZE, BRANCHBOX_SIZE),
            cg, isOpen ? 0 : QStyle::Style_On,
            QStyleOption::Default);
    }
#endif
        Q_UNUSED(w);
        //draw by hand
        p->setPen(KPropertyEditorView::defaultGridLineColor());
        p->drawRect(xmarg, marg, BRANCHBOX_SIZE, BRANCHBOX_SIZE);
        p->fillRect(xmarg + 1, marg + 1, BRANCHBOX_SIZE - 2, BRANCHBOX_SIZE - 2,
                    palette.brush(QPalette::Base));
        p->setPen(palette.color(QPalette::Foreground));
        p->drawLine(xmarg + 2, marg + BRANCHBOX_SIZE / 2, xmarg + BRANCHBOX_SIZE - 3, marg + BRANCHBOX_SIZE / 2);
        if (!isOpen) {
            p->drawLine(xmarg + BRANCHBOX_SIZE / 2, marg + 2,
                        xmarg + BRANCHBOX_SIZE / 2, marg + BRANCHBOX_SIZE - 3);
        }
}

//! @internal
//! Based on KPopupTitle, see kpopupmenu.cpp
class GroupWidgetBase : public QWidget
{
public:
    explicit GroupWidgetBase(QWidget* parent)
            : QWidget(parent)
            , m_isOpen(true)
            , m_mouseDown(false) {
        QSizePolicy sp(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sp.setHorizontalStretch(0);
        sp.setVerticalStretch(1);
        setSizePolicy(sp);
    }

    void setText(const QString &text) {
        m_titleStr = text;
    }

    void setIcon(const QPixmap &pix) {
        m_miniicon = pix;
    }

    virtual bool isOpen() const {
        return m_isOpen;
    }

    virtual void setOpen(bool set) {
        m_isOpen = set;
    }

    QSize sizeHint() const override {
        QSize s(QWidget::sizeHint());
        s.setHeight(fontMetrics().height()*2);
        return s;
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QRect r(rect());
        QPainter p(this);
        QStyleOptionHeader option;
        option.initFrom(this);
        option.state = m_mouseDown ? QStyle::State_Sunken : QStyle::State_Raised;
        style()->drawControl(QStyle::CE_Header, &option, &p, this);

        paintListViewExpander(&p, this, r.height() + 2, palette(), isOpen());
        if (!m_miniicon.isNull()) {
            p.drawPixmap(24, (r.height() - m_miniicon.height()) / 2, m_miniicon);
        }

        if (!m_titleStr.isEmpty()) {
            int indent = 16 + (m_miniicon.isNull() ? 0 : (m_miniicon.width() + 4));
            p.setPen(palette().color(QPalette::Text));
            QFont f = p.font();
            f.setBold(true);
            p.setFont(f);
            p.drawText(indent + 8, 0, width() - (indent + 8),
                       height(), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine,
                       m_titleStr);
        }
    }

    bool event(QEvent * e) override {
        if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            if (me->button() == Qt::LeftButton) {
                m_mouseDown = e->type() == QEvent::MouseButtonPress;
                update();
            }
        }
        return QWidget::event(e);
    }

protected:
    QString m_titleStr;
    QPixmap m_miniicon;
    bool m_isOpen;
    bool m_mouseDown;
};

/*    class GroupWidget : public GroupWidgetBase
    {
    public:
        explicit GroupWidget(EditorGroupItem *parentItem)
                : GroupWidgetBase(parentItem->listView()->viewport())
                , m_parentItem(parentItem) {
        }

        virtual bool isOpen() const {
            return m_parentItem->isOpen();
        }

    protected:
        EditorGroupItem *m_parentItem;
    };*/

class Q_DECL_HIDDEN KPropertyGroupWidget::Private
{
public:
    Private() {}
    QVBoxLayout* lyr;
    GroupWidgetBase *groupWidget;
    QPointer<QWidget> contents;
};

KPropertyGroupWidget::KPropertyGroupWidget(const QString& title, QWidget* parent)
        : QWidget(parent)
        , d(new Private())
{
    QSizePolicy sp(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sp.setHorizontalStretch(0);
    sp.setVerticalStretch(1);
    setSizePolicy(sp);
    d->lyr = new QVBoxLayout(this);
    d->groupWidget = new GroupWidgetBase(this);
    d->groupWidget->setText(title);
    d->lyr->addWidget(d->groupWidget);
    d->lyr->addSpacing(4);
}

KPropertyGroupWidget::~KPropertyGroupWidget()
{
    delete d;
}

void KPropertyGroupWidget::setContents(QWidget* contents)
{
    if (d->contents) {
        d->contents->hide();
        d->lyr->removeWidget(d->contents);
        delete d->contents;
    }
    d->contents = contents;
    if (d->contents) {
        d->lyr->addWidget(d->contents);
        d->contents->show();
    }
    update();
}

bool KPropertyGroupWidget::event(QEvent * e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        QMouseEvent* me = static_cast<QMouseEvent*>(e);
        if (me->button() == Qt::LeftButton && d->contents && d->groupWidget->rect().contains(me->pos())) {
            d->groupWidget->setOpen(!d->groupWidget->isOpen());
            if (d->groupWidget->isOpen())
                d->contents->show();
            else
                d->contents->hide();
            d->lyr->invalidate();
            update();
        }
    }
    return QWidget::event(e);
}

QColor KPropertyUtilsPrivate::contrastColor(const QColor& c)
{
    int g = qGray(c.rgb());
    if (g > 110)
        return c.dark(300);
    else if (g > 80)
        return c.light(250);
    else if (g > 20)
        return c.light(400);
    return Qt::lightGray;
}

QColor KPropertyUtilsPrivate::gridLineColor(const QWidget *widget)
{
    Q_ASSERT(widget);
    KPropertyEditorView *view = nullptr;
    if (widget->parentWidget()) {
        view = qobject_cast<KPropertyEditorView*>(widget->parentWidget()->parentWidget());
    }
    return view ? view->gridLineColor() : QColor();
}

// -------------------

KPROPERTYWIDGETS_EXPORT KProperty* KPropertyUtils::propertyForIndex(const QModelIndex &index)
{
    const KPropertyEditorDataModel *editorModel
            = qobject_cast<const KPropertyEditorDataModel*>(index.model());
    return editorModel ? editorModel->propertyForIndex(index) : nullptr;
}

bool KPropertyUtilsPrivate::shouldUseNativeDialogs()
{
#if defined Q_OS_UNIX && !defined Q_OS_MACOS
    const QString xdgSessionDesktop = QString::fromLatin1(qgetenv("XDG_CURRENT_DESKTOP").trimmed());
    return xdgSessionDesktop.isEmpty()
        && 0 == xdgSessionDesktop.compare(QStringLiteral("KDE"), Qt::CaseInsensitive);
#else
    return true;
#endif
}
