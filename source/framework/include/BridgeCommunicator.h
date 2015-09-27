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

#ifndef SB_BRIDGECOMMUNICATOR_H
#define SB_BRIDGECOMMUNICATOR_H

#include <QObject>
#include <QByteArray>
#include "FrameworkApi.h"

class QLocalSocket;

namespace sb {

class SB_FRAMEWORK_API BridgeCommunicator : public QObject
{
    Q_OBJECT
public:

    enum PacketType {
        PacketType_Request = 1,
        PacketType_Response = 2
    };

    enum State {
        State_NotConnected,
        State_Idle,
        State_ReceiveHeader,
        State_ReceiveBody,
        State_Process,
        State_Error
    };

    BridgeCommunicator(QObject *pParent = nullptr);
    BridgeCommunicator(QLocalSocket *pSocket, QObject *pParent = nullptr);
    ~BridgeCommunicator();

signals:

    void handleNextState();

private slots:

    void onConnected();
    void onDisconnected();
    void onSocketError();
    void onDataAvailable();

    /**
     * @brief Process current state.
     */
    void handleState();

private:

    void initialize();
    void setState(State s);

    static QByteArray encodePacketTypeAndLength(PacketType t, int l);
    static void decodePacketTypeAndLength(const QByteArray &ba, PacketType &t, int &l);

    QLocalSocket *m_pSocket;
    State m_state;
    PacketType m_receivedPacketType;
    int m_receivedPacketLength;

    QByteArray m_receivedPacket;
    int m_receivedLength;
};

} // namespace sb

#endif // SB_BRIDGECOMMUNICATOR_H

