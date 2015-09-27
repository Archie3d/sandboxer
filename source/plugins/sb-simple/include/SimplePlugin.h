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

#ifndef SB_SIMPLE_PLUGIN_H
#define SB_SIMPLE_PLUGIN_H

#include <QtPlugin>
#include "Plugin.h"

class SimplePlugin : public sb::Plugin
{
    Q_OBJECT
    Q_INTERFACES(sb::Plugin)
    Q_PLUGIN_METADATA(IID "sandboxer.plugin" FILE "SimplePlugin.json")

public:

    SimplePlugin(QObject *pParent = nullptr);

    sb::PluginInstance* createInstance(const QVariantMap &config) override;
};

#endif // SB_SIMPLE_PLUGIN_H
