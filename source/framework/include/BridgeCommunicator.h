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

/**
 * @brief Basic IPC communication.
 * This class performs exchange of variant data packets
 * via the local socket (pipe).
 * The variant packet is serialized into a byte array and
 * then deserialized back on receiving side.
 * Serialization byte array is prepended with a header containing
 * the packet type (request or response) and serialization stream length
 * (in bytes).
 */
class SB_FRAMEWORK_API BridgeCommunicator : public QObject
{
    Q_OBJECT
public:

    /**
     * Packet type to differentiate requests and responses.
     */
    enum PacketType {
        PacketType_Request = 1,
        PacketType_Response = 2
    };

    /**
     * Communicator state.
     */
    enum State {
        State_NotConnected,     ///< Local socket is not connected.
        State_Idle,             ///< Waiting for incoming data.
        State_ReceiveHeader,    ///< Receiving message header.
        State_ReceiveBody,      ///< Receiving message body.
        State_Process,          ///< Deserialize message and notify listeners.
        State_Error             ///< Error state.
    };

    BridgeCommunicator(QObject *pParent = nullptr);
    BridgeCommunicator(QLocalSocket *pSocket, QObject *pParent = nullptr);
    ~BridgeCommunicator();

    /**
     * Connect to server socket.
     * @param serverName Server (pipe) name.
     */
    void connectToServer(const QString &serverName);

    /**
     * Close the connection.
     */
    void close();

    /**
     * Tells whether the local socket is currently connected.
     * @return true is socket is connected.
     */
    bool isConnected();

public slots:

    /**
     * Send a packet to connected peer.
     * @param type Packet type.
     * @param data Packet data.
     */
    void sendPacket(PacketType type, const QVariant &data);

signals:

    /// Internal signal, should not be used.
    void handleNextState();

    // Notify on arriving packet
    void packetReceived(PacketType type, const QVariant &data);
    void requestReceived(const QVariant &data);
    void responseReceived(const QVariant &data);

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

    QLocalSocket *m_pSocket;            ///< Local socket (pipe).
    State m_state;                      ///< Current state.
    PacketType m_receivedPacketType;    ///< Type of the received packet.
    int m_receivedPacketLength;         ///< Length of the received packet's binary stream.

    QByteArray m_receivedPacket;        ///< Received packet binary stream.
    int m_receivedLength;               ///< Number of bytes received so far to complete the incoming
                                        ///< packet binary stream.
};

} // namespace sb

#endif // SB_BRIDGECOMMUNICATOR_H

