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

#include <QDebug>
#include <QTimer>
#include <QVBoxLayout>
#include <QMessageBox>
#include "Application.h"
#include "HostWidget.h"
#include "HostPluginInstance.h"

using namespace sb;

HostPluginInstance::HostPluginInstance(const QVariantMap &config)
    : PluginInstance()
{
    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    m_pHostWidget = new HostWidget();
    pLayout->addWidget(m_pHostWidget);
    setLayout(pLayout);

    m_pluginFile = config.value("plugin").toString();

    m_pBridgeServer = new BridgeServer(this);
    connect(m_pBridgeServer, SIGNAL(requestReceived(QVariant)), this, SLOT(onRequestReceived(QVariant)));

    // Spawn a process sbprocess
    // sbprocess -xpipe <pipename> -plugin <pluginName>

    m_pProcess = new QProcess(this);
    connect(m_pProcess, &QProcess::started, this, &HostPluginInstance::onProcessStarted);
    connect(m_pProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    connect(m_pProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));
    connect(m_pProcess, &QProcess::readyReadStandardOutput, this, &HostPluginInstance::onProcessStdOutput);
    connect(m_pProcess, &QProcess::readyReadStandardError, this, &HostPluginInstance::onProcessStdError);

    QTimer::singleShot(0, this, &HostPluginInstance::start);
}

HostPluginInstance::~HostPluginInstance()
{
    exitHostedProcess();
}

void HostPluginInstance::start()
{
    Q_ASSERT(m_pProcess->state() == QProcess::NotRunning);

    // Start the bridge server
    m_pBridgeServer->start();

    QStringList args;
    args << "-x" << m_pBridgeServer->pipeName();
    args << "-p" << m_pluginFile;

    m_pProcess->setWorkingDirectory(Application::instance()->applicationDirPath());
    m_pProcess->start("sbprocess.exe", args);
}

void HostPluginInstance::onProcessStarted()
{
    qDebug() << "Sabdbox process started";
}

void HostPluginInstance::onProcessError(QProcess::ProcessError err)
{
    qDebug() << "Sandbox process error" << err;
}

void HostPluginInstance::onProcessFinished(int code, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::Crashed || code != 0) {
        int ret = QMessageBox::question(this, tr("Crash"),
                                        tr("The plugin %1 has terminated prematurely. Restart?").arg(m_pluginFile),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            // Restart the process
            start();
        }
    } else {
        qDebug() << "Sandbox process finished normally";
    }
}

void HostPluginInstance::onProcessStdOutput()
{
    QString str = m_pProcess->readAllStandardOutput();
    if (!str.isEmpty()) {
        qDebug() << "[SANDBOX]" << str.trimmed();
    }
}

void HostPluginInstance::onProcessStdError()
{
    QString str = m_pProcess->readAllStandardError();
    if (!str.isEmpty()) {
        qWarning() << "[SANDBOX]" << str.trimmed();
    }
}

void HostPluginInstance::onRequestReceived(const QVariant &data)
{
    QVariantMap map = data.toMap();
    if (map["name"] == "setWinId") {
        embedWindow(map["winid"].toInt());
    }
}

void HostPluginInstance::embedWindow(int id)
{
    qDebug() << "Embedding window id=" << id;
    m_pHostWidget->attachChildWindow(id);

    // Trigger show event
    QVariantMap msg;
    msg["name"] = "show";
    m_pBridgeServer->sendRequest(msg);
}

void HostPluginInstance::exitHostedProcess()
{
    QVariantMap msg;
    msg["name"] = "exit";
    m_pBridgeServer->sendRequest(msg);

    m_pHostWidget->detachChildWindow();
    m_pProcess->terminate();
    m_pProcess->waitForFinished(3000);
    if (m_pProcess->state() != QProcess::NotRunning) {
        m_pProcess->kill();
    }
}
