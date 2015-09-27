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

#ifndef SB_APPLICATION_H
#define SB_APPLICATION_H

#include <QApplication>
#include "FrameworkApi.h"

namespace sb {

class PluginsManager;

class SB_FRAMEWORK_API Application : public QApplication
{
    Q_OBJECT
public:

    static Application* instance();

    Application(int &argc, char **argv);
    ~Application();

    void registerPlugin(const QString &fileName);

    PluginsManager* pluginsManager() const { return m_pPluginsManager; }

public slots:

    int launch();

signals:

    void initialized();

private slots:

    void initialize();

private:

    static Application *s_pInstance;

    PluginsManager *m_pPluginsManager;
    QStringList m_pluginsToLoad;

};

} // namespace sb

#endif // SB_APPLICATION_H

