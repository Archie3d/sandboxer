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
#include "Application.h"
#include "HostPluginInstance.h"

using namespace sb;

HostPluginInstance::HostPluginInstance(const QVariantMap &config)
    : PluginInstance()
{
    m_pluginFile = config.value("plugin").toString();

    // TODO: spawn a process sbprocess
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
    m_pProcess->terminate();
    m_pProcess->waitForFinished(3000);
    if (m_pProcess->state() != QProcess::NotRunning) {
        m_pProcess->kill();
    }
}

void HostPluginInstance::start()
{
    Q_ASSERT(m_pProcess->state() == QProcess::NotRunning);

    QStringList args;
    args << "-xpipe" << "bridge_pipe_name";
    args << "-plugin" << m_pluginFile;

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
    if (exitStatus == QProcess::Crashed) {
        qDebug() << "Sandbox process crashed" << code;
    } else {
        qDebug() << "Sandbox process finished" << code;
    }
}

void HostPluginInstance::onProcessStdOutput()
{
}

void HostPluginInstance::onProcessStdError()
{
}
