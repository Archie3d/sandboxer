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
#include <QPluginLoader>
#include "Plugin.h"
#include "PluginsManager.h"

namespace sb {

PluginsManager::PluginsManager(QObject *pParent)
    : QObject(pParent),
      m_plugins()
{
}

PluginsManager::~PluginsManager()
{
    // Unload all plugins
    foreach (const QString &name, m_plugins.keys()) {
        unload(name);
    }
}


Plugin *PluginsManager::load(const QString &absolutePath)
{
    QPluginLoader *pLoader = new QPluginLoader(absolutePath);
    QJsonObject obj = pLoader->metaData().value("MetaData").toObject();

    QString name = obj.value("name").toString();
    if (name.isEmpty()) {
        delete pLoader;
        return nullptr;
    }

    Plugin *pPlugin = qobject_cast<Plugin*>(pLoader->instance());
    if (pPlugin != nullptr) {
        pPlugin->setName(name);
        pPlugin->initialize();
        m_plugins.insert(name, pLoader);
        qDebug() << "Loaded plugin" << name << "from" << absolutePath;
    } else {
        qWarning() << "Unable to load" << absolutePath;
        delete pLoader;
    }

    return pPlugin;
}

void PluginsManager::unload(const QString &name)
{
    QPluginLoader *pLoader = m_plugins.value(name);
    if (pLoader == nullptr) {
        return;
    }

    Plugin *pPlugin = qobject_cast<Plugin*>(pLoader->instance());
    if (pPlugin != nullptr) {
        pPlugin->cleanup();
    }

    if (pLoader->unload()) {
        qDebug() << name << "plugin has been unloaded";
        m_plugins.remove(name);
        delete pLoader;
    } else {
        qWarning() << "Unable to unload" << name;
    }
}

PluginInstance* PluginsManager::createInstance(const QString &name, const QVariantMap &config)
{
    QPluginLoader *pLoader = m_plugins.value(name);
    if (pLoader == nullptr) {
        qCritical() << "Unable to find plugin" << name;
        return nullptr;
    }

    Plugin *pPlugin = qobject_cast<Plugin*>(pLoader->instance());
    Q_ASSERT(pPlugin != nullptr);

    return pPlugin->createInstance(config);
}

} // namespace sb
