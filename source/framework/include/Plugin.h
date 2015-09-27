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

class SB_FRAMEWORK_API Plugin : public QObject
{
    Q_OBJECT

    friend class PluginsManager;

public:

    Plugin(QObject *pParent = nullptr);

    QString name() const { return m_name; }

    virtual void initialize();
    virtual void cleanup();
    virtual PluginInstance* createInstance(const QVariantMap &config) = 0;

protected:

    void setName(const QString &n) { m_name = n; }

private:

    QString m_name; ///< Plugin name.
};

} // namespace sb

Q_DECLARE_INTERFACE(sb::Plugin, "sandboxer.plugin")
