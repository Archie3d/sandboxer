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

#ifndef HOSTEDAPPLICATION_H
#define HOSTEDAPPLICATION_H

#include <QApplication>
#include "BridgeClient.h"
#include "PluginsManager.h"

class HostedApplication : public QApplication
{
    Q_OBJECT
public:

    HostedApplication(int &argc, char **argv);
    ~HostedApplication();

    int launch();

private slots:

    void loadPlugin();
    void initBridge();
    void handleRequest(const QVariant &data);


    // Request to host bridge
    void sendWinId();

private:

    bool parseArguments();

    sb::PluginsManager *m_pPluginsManager;
    sb::BridgeClient *m_pBridgeClient;
    sb::PluginInstance *m_pPluginInstance;
    QWidget *m_pTopWidget;
    QString m_pipeName;
    QString m_pluginName;
};

#endif // HOSTEDAPPLICATION_H

