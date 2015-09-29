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

#include <QDate>
#include <QTime>
#include <QCryptographicHash>
#include <QLocalServer>
#include "BridgeCommunicator.h"
#include "BridgeServer.h"

namespace sb {

BridgeServer::BridgeServer(QObject *pParent)
    : QObject(pParent)
{
    m_pipeName = generateRandomPipeName();
    m_pLocalServer = new QLocalServer(this);
    m_pLocalServer->setMaxPendingConnections(1);
    connect(m_pLocalServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    m_pBridgeCommunicator = nullptr;
}

void BridgeServer::start()
{
    if (m_pLocalServer->isListening()) {
        // Already started
        return;
    }

    m_pLocalServer->listen(m_pipeName);
}

void BridgeServer::sendRequest(const QVariant &data)
{
    m_pBridgeCommunicator->sendPacket(BridgeCommunicator::PacketType_Request, data);
}

void BridgeServer::sendResponse(const QVariant &data)
{
    m_pBridgeCommunicator->sendPacket(BridgeCommunicator::PacketType_Response, data);
}

void BridgeServer::onNewConnection()
{
    QLocalSocket *pSocket = m_pLocalServer->nextPendingConnection();

    delete m_pBridgeCommunicator;
    m_pBridgeCommunicator = new BridgeCommunicator(pSocket, this);
    connect(m_pBridgeCommunicator, SIGNAL(requestReceived(QVariant)), this, SIGNAL(requestReceived(QVariant)));
    connect(m_pBridgeCommunicator, SIGNAL(responseReceived(QVariant)), this, SIGNAL(responseReceived(QVariant)));
}

QString BridgeServer::generateRandomPipeName()
{
    QString str = QDate::currentDate().toString()
            + QTime::currentTime().toString()
            + QString::number(qrand());
    QByteArray data = str.toUtf8();
    QByteArray ba = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    QString hash = QString(ba.toHex()).toLower();
    return hash;
}

} // namespace sb
