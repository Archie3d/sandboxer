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

#ifndef SB_BRIDGECLIENT_H
#define SB_BRIDGECLIENT_H

#include <QObject>
#include "FrameworkApi.h"
#include "BridgeCommunicator.h"

namespace sb {

class SB_FRAMEWORK_API BridgeClient : public QObject
{
    Q_OBJECT
public:

    BridgeClient(QObject *pParent = nullptr);

    void connectToServer(const QString &serverName);
    bool isConnected();
    void close();

    void sendRequest(const QVariant &data);

signals:

    void requestReceived(const QVariant &data);
    void responseReceived(const QVariant &data);

private:

    BridgeCommunicator *m_pBridgeCommunicator;
};

} // namespace sb

#endif // SB_BRIDGECLIENT_H

