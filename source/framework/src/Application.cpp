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

#include <QDir>
#include <QTimer>
#include "PluginsManager.h"
#include "Application.h"

namespace sb {

Application* Application::s_pInstance = nullptr;

Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
{
    Q_ASSERT(s_pInstance == nullptr);

    m_pPluginsManager = new PluginsManager(this);

    s_pInstance = this;
}

Application::~Application()
{
    s_pInstance = nullptr;
}

void Application::registerPlugin(const QString &fileName)
{
    QDir appDir(applicationDirPath());
    m_pluginsToLoad.append(appDir.absoluteFilePath(fileName));
}

Application* Application::instance()
{
    return s_pInstance;
}

int Application::launch()
{
    QTimer::singleShot(0, this, &Application::initialize);
    return exec();
}

void Application::initialize()
{
    // Load registered plugins
    foreach (const QString &file, m_pluginsToLoad) {
        m_pPluginsManager->load(file);
    }
    m_pluginsToLoad.clear();

    // Notofy application has been initialized
    emit initialized();
}

} // namespace sb
