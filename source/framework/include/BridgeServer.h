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

#ifndef SB_BRIDGESERVER_H
#define SB_BRIDGESERVER_H

#include <QObject>
#include "FrameworkApi.h"

class QLocalServer;

namespace sb {

class BridgeCommunicator;

class SB_FRAMEWORK_API BridgeServer : public QObject
{
    Q_OBJECT
public:

    BridgeServer(QObject *pParent = nullptr);

    QString pipeName() const { return m_pipeName; }

    void start();
    void sendRequest(const QVariant &data);
    void sendResponse(const QVariant &data);

signals:

    void requestReceived(const QVariant &data);
    void responseReceived(const QVariant &data);

private slots:

    void onNewConnection();

private:

    static QString generateRandomPipeName();

    QString m_pipeName;
    QLocalServer *m_pLocalServer;
    BridgeCommunicator *m_pBridgeCommunicator;
};

} // namespace sb

#endif // SB_BRIDGESERVER_H

