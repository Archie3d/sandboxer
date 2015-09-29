/*
                          sandboxer

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#include <QtGlobal>

#ifdef Q_OS_WIN
#   include <Windows.h>
#endif

#include <QDebug>
#include <QTimer>

#include "HostWidget.h"

HostWidget::HostWidget(QWidget *pParent)
    : QWidget(pParent)
{
    unsetCursor();

    m_childWindowId = 0;
    m_originalParentWindowId = 0;
    m_originalWindowStyle = 0;
}

HostWidget::~HostWidget()
{
    detachChildWindow();
}

void HostWidget::attachChildWindow(WindowId wid)
{
    detachChildWindow();

    if (wid == 0) {
        return;
    }
#ifdef Q_OS_WIN

    HWND hWnd = (HWND)wid;
    m_originalWindowStyle = ::GetWindowLongW(hWnd, GWL_STYLE);

    long style(m_originalWindowStyle);

    style |= WS_CHILD;
    style &= ~WS_POPUP;
    style &= ~WS_CAPTION;
    style &= ~WS_THICKFRAME;

    m_originalParentWindowId = (WindowId)(::GetParent(hWnd));

    ::SetWindowLongW(hWnd, GWL_STYLE, style);
    ::SetParent(hWnd, (HWND)winId());
    ::SetWindowPos(hWnd, HWND_TOP, 0, 0, width(), height(), SWP_SHOWWINDOW);
    ::ShowWindow(hWnd, SW_SHOW);

#endif

    m_childWindowId = wid;

    // Trigger update by slightly resizing the window
    QTimer::singleShot(10, [this]() {
        QSize s = size();
        resize(s.width() + 1, s.height() + 1);
        resize(s);
    });

}

void HostWidget::detachChildWindow()
{
    if (m_childWindowId == 0) {
        return;
    }

#ifdef Q_OS_WIN
    HWND hWnd = (HWND)m_childWindowId;
    ::ShowWindow(hWnd, SW_HIDE);
    ::SetWindowLongW(hWnd, GWL_STYLE, m_originalWindowStyle);
    ::SetParent(hWnd, (HWND)m_originalParentWindowId);
#endif

    m_childWindowId = 0;
    m_originalParentWindowId = 0;
    m_originalWindowStyle = 0;
}

void HostWidget::adjustChildWindowSize()
{
#ifdef Q_OS_WIN
    if (m_childWindowId != 0) {
        ::SetWindowPos((HWND)m_childWindowId,
                       HWND_TOP, 0, 0, width(), height(), 0);
    }
#endif
}

void HostWidget::showEvent(QShowEvent *pEvent)
{
    QWidget::showEvent(pEvent);
#ifdef Q_OS_WIN
    if (m_childWindowId != 0) {
        ::SetWindowPos((HWND)m_childWindowId,
                       HWND_TOP, 0, 0, width(), height(), SWP_SHOWWINDOW);
    }
#endif
}

void HostWidget::focusInEvent(QFocusEvent *pEvent)
{
    QWidget::focusInEvent(pEvent);
#ifdef Q_OS_WIN
    if (m_childWindowId != 0) {
        ::SetFocus((HWND)m_childWindowId);
    }
#endif
}

void HostWidget::moveEvent(QMoveEvent *pEvent)
{
    QWidget::moveEvent(pEvent);
#ifdef Q_OS_WIN
    if (m_childWindowId != 0) {
        ::SetWindowPos((HWND)m_childWindowId,
                       HWND_TOP, 0, 0, width(), height(), 0);
    }
#endif
}

void HostWidget::resizeEvent(QResizeEvent *pEvent)
{
    QWidget::resizeEvent(pEvent);
#ifdef Q_OS_WIN
    if (m_childWindowId != 0) {
        ::SetWindowPos((HWND)m_childWindowId,
                       HWND_TOP, 0, 0, width(), height(), 0);
    }
#endif
}
