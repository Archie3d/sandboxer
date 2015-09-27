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

#include <iostream>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QCommandLineParser>
#include "Plugin.h"
#include "PluginInstance.h"
#include "../include/HostedApplication.h"

using namespace sb;

HostedApplication::HostedApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setAttribute(Qt::AA_NativeWindows, true);

    m_pPluginsManager = new PluginsManager(this);
}

HostedApplication::~HostedApplication()
{

}

int HostedApplication::launch()
{
    if (!parseArguments()) {
        return -1;
    }

    QTimer::singleShot(0, this, &HostedApplication::loadPlugin);

    return exec();
}

void HostedApplication::loadPlugin()
{
    Q_ASSERT(m_pPluginsManager != nullptr);

    QDir appPath(applicationDirPath());

    Plugin *pPlugin = m_pPluginsManager->load(appPath.absoluteFilePath(m_pluginName));
    if (pPlugin == nullptr) {
        exit(-1);
        return;
    }

    QVariantMap cfg;
    PluginInstance *pInstance = pPlugin->createInstance(cfg);
    if (pInstance == nullptr) {
        exit(-1);
        return;
    }

    pInstance->show();
    pInstance->setWindowTitle(QString("%1").arg(pInstance->winId()));

    std::cout << pInstance->winId() << std::endl;
}

bool HostedApplication::parseArguments()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Sandbox hosted process");

    QCommandLineOption optPipe(QStringList() << "x" << "xpipe", tr("IPC pipe name"), "pipe");
    QCommandLineOption optPlugin(QStringList() << "p" << "xplugin", tr("Hosted plugin"), "plugin");
    parser.addOption(optPipe);
    parser.addOption(optPlugin);

    if (!parser.parse(arguments())) {
        qCritical() << "Unable to parse arguments";
        return false;
    }

    if (!parser.isSet(optPipe)) {
        qCritical() << "Pipe name is not set";
        return false;
    }

    if (!parser.isSet(optPlugin)) {
        qCritical() << "Plugin name is not set";
        return false;
    }

    m_pipeName = parser.value(optPipe);
    m_pluginName = parser.value(optPlugin);

    if (m_pipeName.isEmpty()) {
        return false;
    }

    if (m_pluginName.isEmpty()) {
        return false;
    }

    return true;
}
