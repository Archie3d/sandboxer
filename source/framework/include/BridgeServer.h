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

/**
 * @brief IPC server.
 * This class creates a socket (pipe) server and listens
 * for incoming connections. Only single peer connection is handled.
 * The name of the communication pipe is randomly generated (an MD5 hash)
 * upon the server start.
 */
class SB_FRAMEWORK_API BridgeServer : public QObject
{
    Q_OBJECT
public:

    /**
     * Construct a server.
     * This will not yet start accepting connection.
     * @param pParent
     */
    BridgeServer(QObject *pParent = nullptr);

    /**
     * Returns IPC pipe name.
     * @return Pipe name.
     */
    QString pipeName() const { return m_pipeName; }

    /**
     * Start listening for incoming connections.
     */
    void start();

    /**
     * Send request packet to connected peer.
     * @param data Request data.
     */
    void sendRequest(const QVariant &data);

    /**
     * Send response packet to connected peer.
     * @param data Response data.
     */
    void sendResponse(const QVariant &data);

signals:

    /**
     * Request packet received from connected peer.
     * @param data
     */
    void requestReceived(const QVariant &data);

    /**
     * Response packet received from connected peer.
     * @param data
     */
    void responseReceived(const QVariant &data);

private slots:

    void onNewConnection();

private:

    /**
     * Generate random pipe name using MD5 hash.
     * @return
     */
    static QString generateRandomPipeName();

    QString m_pipeName;     ///< IPC pipe name.
    QLocalServer *m_pLocalServer;   ///< Local server object.

    /// Communication object used to talk to peer.
    BridgeCommunicator *m_pBridgeCommunicator;
};

} // namespace sb

#endif // SB_BRIDGESERVER_H

