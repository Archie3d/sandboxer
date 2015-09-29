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

#include "BridgeClient.h"

namespace sb {

BridgeClient::BridgeClient(QObject *pParent)
    : QObject(pParent)
{
    m_pBridgeCommunicator = new BridgeCommunicator(this);
    connect(m_pBridgeCommunicator, SIGNAL(requestReceived(QVariant)), this, SIGNAL(requestReceived(QVariant)));
    connect(m_pBridgeCommunicator, SIGNAL(responseReceived(QVariant)), this, SIGNAL(responseReceived(QVariant)));
}

void BridgeClient::connectToServer(const QString &serverName)
{
    m_pBridgeCommunicator->connectToServer(serverName);
}

bool BridgeClient::isConnected()
{
    return m_pBridgeCommunicator->isConnected();
}

void BridgeClient::close()
{
    m_pBridgeCommunicator->close();
}

void BridgeClient::sendRequest(const QVariant &data)
{
    m_pBridgeCommunicator->sendPacket(BridgeCommunicator::PacketType_Request, data);
}

void BridgeClient::sendResponse(const QVariant &data)
{
    m_pBridgeCommunicator->sendPacket(BridgeCommunicator::PacketType_Response, data);
}

} // namespace sb
