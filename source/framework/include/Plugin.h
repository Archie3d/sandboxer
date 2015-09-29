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

#include <QObject>
#include <QWidget>
#include "FrameworkApi.h"

namespace sb {

class PluginInstance;

/**
 * @brief Plugin abstract object.
 * A plugin can generate as many instanced as requested.
 * Each instance can be configured differently upon creation.
 */
class SB_FRAMEWORK_API Plugin : public QObject
{
    Q_OBJECT

    friend class PluginsManager;

public:

    Plugin(QObject *pParent = nullptr);

    QString name() const { return m_name; }

    /**
     * @brief Initialize the plugin.
     * Called by @ref PluginsManager just after the plugin has been loaded.
     */
    virtual void initialize();

    /**
     * @brief Cleanup the plugin.
     * Called by @ref PluginsManager before unloading the plugin.
     */
    virtual void cleanup();

    /**
     * Create plugin's instance.
     * @param config Instance configuration data.
     * @return Create plugin or null if error.
     */
    virtual PluginInstance* createInstance(const QVariantMap &config) = 0;

protected:

    /**
     * @brief Assign plugin name.
     * Name is assigned by @ref PluginsManager that fetches it in its turn
     * from plugin's metadata (json object).
     * @param n
     */
    void setName(const QString &n) { m_name = n; }

private:

    QString m_name; ///< Plugin name.
};

} // namespace sb

Q_DECLARE_INTERFACE(sb::Plugin, "sandboxer.plugin")
