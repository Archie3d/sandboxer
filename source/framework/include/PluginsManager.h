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

class SB_FRAMEWORK_API PluginsManager : public QObject
{
    Q_OBJECT
public:

    PluginsManager(QObject *pParent = nullptr);
    ~PluginsManager();

    Plugin* load(const QString &absolutePath);
    void unload(const QString &name);

    PluginInstance* createInstance(const QString &name, const QVariantMap &config = QVariantMap());

private:
    // Map plugin name to its loader.
    QMap<QString, QPluginLoader*> m_plugins;
};

} // namespace sb

#endif // SB_PLUGINSMANAGER_H

