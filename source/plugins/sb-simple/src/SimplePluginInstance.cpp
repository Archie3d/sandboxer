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
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include "SimplePluginInstance.h"

using namespace sb;

SimplePluginInstance::SimplePluginInstance(const QVariantMap &config)
    : PluginInstance()
{
    QVBoxLayout *pLayout = new QVBoxLayout();
    QLabel *pLabel = new QLabel(tr("Simple plugin"));
    QTextEdit *pEdit = new QTextEdit();
    QPushButton *pButton = new QPushButton(tr("Crash me"));
    connect(pButton, SIGNAL(clicked(bool)), this, SLOT(crash()));
    pLayout->addWidget(pLabel);
    pLayout->addWidget(pEdit);
    pLayout->addWidget(pButton);
    setLayout(pLayout);
}

void SimplePluginInstance::crash()
{
    int *ptr = (int*)0;
    int x = *ptr;
    if (x == 0) {
        qDebug() << "Unreachable code";
    }
}
