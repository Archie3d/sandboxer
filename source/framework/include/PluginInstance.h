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

#ifndef SB_PLUGININSTANCE_H
#define SB_PLUGININSTANCE_H

#include <QWidget>
#include "FrameworkApi.h"

namespace sb {

class SB_FRAMEWORK_API PluginInstance : public QWidget
{
    Q_OBJECT
public:

    PluginInstance(QWidget *pParent = nullptr);
};

} // namespace sb

#endif // SB_PLUGININSTANCE_H

