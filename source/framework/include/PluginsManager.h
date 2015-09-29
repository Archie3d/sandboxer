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

#ifndef SB_PLUGINSMANAGER_H
#define SB_PLUGINSMANAGER_H

#include <QObject>
#include <QMap>
#include "FrameworkApi.h"

class QPluginLoader;

namespace sb {

class Plugin;
class PluginInstance;

/**
 * @brief Handle plugins loading and unloading.
 *
 * This class handled plugins loading and unloading.
 * It also acts as facade to particular plugins to create
 * plugin instances.
 */
class SB_FRAMEWORK_API PluginsManager : public QObject
{
    Q_OBJECT
public:

    /**
     * Construct plugins manager but do not load any plugins.
     * @param pParent
     */
    PluginsManager(QObject *pParent = nullptr);

    /**
     * Unload all plugins.
     * @note There is no guarantee that all the plugins will be unloaded
     *     after destruction of the manager.
     */
    ~PluginsManager();

    /**
     * Load plugin.
     * @param absolutePath Plugin library absolute path.
     * @return Pointer to loaded plugin or null if error.
     */
    Plugin* load(const QString &absolutePath);

    /**
     * Unload plugin by its name.
     * @param name Plugin's name.
     */
    void unload(const QString &name);

    /**
     * Create plugin's instance.
     * @param name Plugin name.
     * @param config Plugin instance configuration data.
     * @return Pointer to new instance or null if error.
     */
    PluginInstance* createInstance(const QString &name, const QVariantMap &config = QVariantMap());

private:

    /// Map plugin names to their loaders.
    QMap<QString, QPluginLoader*> m_plugins;
};

} // namespace sb

#endif // SB_PLUGINSMANAGER_H

