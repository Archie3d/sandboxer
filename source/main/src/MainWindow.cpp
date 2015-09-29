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

#include <QHBoxLayout>
#include <QSplitter>
#include "Application.h"
#include "PluginInstance.h"
#include "PluginsManager.h"
#include "../include/MainWindow.h"

using namespace sb;

MainWindow::MainWindow(QWidget *pParent)
    : QMainWindow(pParent)
{
    connect(Application::instance(), SIGNAL(initialized()), this, SLOT(initialize()));
}

void MainWindow::initialize()
{
    QWidget *pWidget = new QWidget();
    QHBoxLayout *pLayout = new QHBoxLayout();

    pWidget->setLayout(pLayout);

    QSplitter *pSplitter = new QSplitter(Qt::Horizontal);
    pLayout->addWidget(pSplitter);

    PluginInstance *pNativeInstance = Application::instance()->pluginsManager()->createInstance("Simple");
    if (pNativeInstance != nullptr) {        
        pSplitter->addWidget(pNativeInstance);
    }

    QVariantMap cfg = {
         { "plugin", "sb-simple.dll" }
    };
    PluginInstance *pSbInstance = Application::instance()->pluginsManager()->createInstance("Host", cfg);
    if (pSbInstance != nullptr) {
        pSplitter->addWidget(pSbInstance);
    }

    int s = width() / 2;
    pSplitter->setSizes(QList<int>() << s << s);

    setCentralWidget(pWidget);
}
