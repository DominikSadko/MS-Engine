#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "framework/core/platform.h"
#include <stdex/buffer.h>

class Connection;

class Client
{
    public:
        Client(TCPsocket socket, Connection* connection);
        ~Client();

        bool onReceiveMessage(int32_t size);

        Buffer* getInputBuffer() const;
        Buffer* getOutputBuffer() const;

        TCPsocket getSocket() const { return m_socket; }
        Connection* getConnection() const { return m_connection; }

    private:
        TCPsocket m_socket;
        Connection* m_connection;
};

#endif // __CLIENT_H__ //
