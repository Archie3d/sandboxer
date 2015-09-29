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

#include <QLocalSocket>
#include <QDataStream>
#include "BridgeCommunicator.h"

namespace sb {

BridgeCommunicator::BridgeCommunicator(QObject *pParent)
    : QObject(pParent)
{
    m_pSocket = new QLocalSocket(this);
    initialize();
}

BridgeCommunicator::BridgeCommunicator(QLocalSocket *pSocket, QObject *pParent)
    : QObject(pParent)
{
    Q_ASSERT(pSocket != nullptr);
    m_pSocket = pSocket;
    initialize();
}

BridgeCommunicator::~BridgeCommunicator()
{
    close();
}

void BridgeCommunicator::connectToServer(const QString &serverName)
{
    close();
    m_pSocket->connectToServer(serverName);
}

void BridgeCommunicator::close()
{
    Q_ASSERT(m_pSocket != nullptr);
    m_pSocket->close();
}

bool BridgeCommunicator::isConnected()
{
    Q_ASSERT(m_pSocket != nullptr);
    return m_pSocket->isOpen();
}

void BridgeCommunicator::sendPacket(PacketType type, const QVariant &data)
{
    Q_ASSERT(m_pSocket != nullptr);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << data;

    QByteArray packet = encodePacketTypeAndLength(type, buffer.length());
    packet = packet.append(buffer);
    qint64 s = m_pSocket->write(packet);
    Q_ASSERT(s == packet.size());
}

void BridgeCommunicator::onConnected()
{
    setState(State_Idle);
}

void BridgeCommunicator::onDisconnected()
{
    setState(State_NotConnected);
}

void BridgeCommunicator::onSocketError()
{
    qWarning() << "Socket error";
}

void BridgeCommunicator::onDataAvailable()
{
    if (m_state == State_Idle) {
        setState(State_ReceiveHeader);
    } else {
        handleNextState();
    }
}

void BridgeCommunicator::handleState()
{
    switch (m_state) {
    case State_NotConnected:
        break;
    case State_Idle:
        if (m_pSocket->bytesAvailable() > 0) {
            setState(State_ReceiveHeader);
        }
        break;
    case State_ReceiveHeader: {
        if (m_pSocket->bytesAvailable() < 8) {
            // Header requires 8 bytes
            return;
        }

        QByteArray ba = m_pSocket->read(8);
        decodePacketTypeAndLength(ba, m_receivedPacketType, m_receivedPacketLength);

        m_receivedPacket.clear();
        m_receivedLength = 0;

        setState(State_ReceiveBody);
        break;
    }
    case State_ReceiveBody: {

        int remained = m_receivedPacketLength - m_receivedLength;
        QByteArray ba = m_pSocket->read(remained);
        m_receivedPacket.append(ba);
        m_receivedLength += ba.length();

        if (m_receivedLength == m_receivedPacketLength) {
            // Complete packet received
            setState(State_Process);
        }

        break;
    }
    case State_Process: {
        // Decode packet
        QDataStream stream(&m_receivedPacket, QIODevice::ReadOnly);
        QVariant data;
        stream >> data;

        emit packetReceived(m_receivedPacketType, data);
        if (m_receivedPacketType == PacketType_Request) {
            emit requestReceived(data);
        } else {
            emit responseReceived(data);
        }

        setState(State_Idle);
        break;
    }
    case State_Error:
        break;
    default:
        Q_ASSERT(!"Invalid state");
    }
}

void BridgeCommunicator::initialize()
{
    Q_ASSERT(m_pSocket != nullptr);

    m_state = State_NotConnected;
    if (m_pSocket->isOpen()) {
        m_state = State_Idle;
    }
    connect(this, SIGNAL(handleNextState()), this, SLOT(handleState()), Qt::QueuedConnection);

    m_pSocket->setParent(this);
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_pSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onSocketError()));
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

void BridgeCommunicator::setState(State s)
{
    m_state = s;
    emit handleNextState();
}

QByteArray BridgeCommunicator::encodePacketTypeAndLength(PacketType t, int l)
{
    char p[8];

    int tp = (int)t;

    p[0] = tp & 0xFF;
    p[1] = (tp >> 8) & 0xFF;
    p[2] = (tp >> 16) & 0xFF;
    p[3] = (tp >> 24) & 0xFF;

    p[4] = l & 0xFF;
    p[5] = (l >> 8) & 0xFF;
    p[6] = (l >> 16) & 0xFF;
    p[7] = (l >> 24) & 0xFF;

    return QByteArray(p, 8);
}

void BridgeCommunicator::decodePacketTypeAndLength(const QByteArray &ba, PacketType &t, int &l)
{
    Q_ASSERT(ba.length() == 8);

    int res = 0;
    res = ba.at(3);
    res = (res << 8) | ba.at(2);
    res = (res << 8) | ba.at(1);
    res = (res << 8) | ba.at(0);
    t = (PacketType)res;

    res = 0;
    res = (unsigned char)ba.at(7);
    res = (res << 8) | (unsigned char)ba.at(6);
    res = (res << 8) | (unsigned char)ba.at(5);
    res = (res << 8) | (unsigned char)ba.at(4);

    l = res;
}

} // namespace sb
