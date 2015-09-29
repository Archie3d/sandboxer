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

#ifndef HOSTWIDGET_H
#define HOSTWIDGET_H

#include <QWidget>

class HostWidget : public QWidget
{
    Q_OBJECT
public:

    typedef unsigned int WindowId;

    HostWidget(QWidget *pParent = 0);
    ~HostWidget();

    void attachChildWindow(WindowId wid);
    void detachChildWindow();
    void adjustChildWindowSize();

protected:

    void showEvent(QShowEvent *pEvent);
    void focusInEvent(QFocusEvent *pEvent);
    void moveEvent(QMoveEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);

private:

    WindowId m_childWindowId;
    WindowId m_originalParentWindowId;
    long m_originalWindowStyle;
};

#endif // HOSTWIDGET_H

