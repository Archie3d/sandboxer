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
#include <QVBoxLayout>
#include "Plugin.h"
#include "PluginInstance.h"
#include "../include/HostedApplication.h"

using namespace sb;

HostedApplication::HostedApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setAttribute(Qt::AA_NativeWindows, true);    
    m_pPluginsManager = new PluginsManager(this);
    m_pBridgeClient = new BridgeClient(this);
    connect(m_pBridgeClient, SIGNAL(requestReceived(QVariant)), this, SLOT(handleRequest(QVariant)));
    m_pPluginInstance = nullptr;
}

HostedApplication::~HostedApplication()
{
    delete m_pTopWidget;
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
    m_pPluginInstance = pPlugin->createInstance(cfg);
    if (m_pPluginInstance == nullptr) {
        exit(-1);
        return;
    }

    m_pTopWidget = new QWidget(nullptr, Qt::WindowFlags(Qt::FramelessWindowHint));
    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    m_pTopWidget->setLayout(pLayout);
    pLayout->addWidget(m_pPluginInstance);

    initBridge();
}

void HostedApplication::initBridge()
{
    m_pBridgeClient->connectToServer(m_pipeName);
    QTimer::singleShot(0, this, &HostedApplication::sendWinId);
}

void HostedApplication::sendWinId()
{
    Q_ASSERT(m_pPluginInstance != nullptr);

    QVariantMap msg;
    msg["name"] = "setWinId";
    msg["winid"] = m_pTopWidget->winId();

    m_pBridgeClient->sendRequest(msg);
}

void HostedApplication::handleRequest(const QVariant &data)
{
    QVariantMap map = data.toMap();
    QString name = map["name"].toString();
    if (name == "show") {
        m_pTopWidget->setVisible(true);
    } else if (name == "hide") {
        m_pTopWidget->setVisible(false);
    } else if (name == "exit") {
        exit(0);
    }
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
