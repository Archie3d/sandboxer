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

#ifndef HOSTPLUGININSTANCE_H
#define HOSTPLUGININSTANCE_H

#include <QProcess>
#include "BridgeServer.h"
#include "PluginInstance.h"

class HostWidget;

class HostPluginInstance : public sb::PluginInstance
{
    Q_OBJECT
public:

    HostPluginInstance(const QVariantMap &config);
    ~HostPluginInstance();

private slots:

    void start();

    void onProcessStarted();
    void onProcessError(QProcess::ProcessError err);
    void onProcessFinished(int code, QProcess::ExitStatus exitStatus);
    void onProcessStdOutput();
    void onProcessStdError();

    void onRequestReceived(const QVariant &data);
    void embedWindow(int id);
    void exitHostedProcess();

private:

    // Process running sbprocess
    QProcess *m_pProcess;
    sb::BridgeServer *m_pBridgeServer;
    HostWidget *m_pHostWidget;
    QString m_pluginFile;
};

#endif // HOSTPLUGININSTANCE_H

