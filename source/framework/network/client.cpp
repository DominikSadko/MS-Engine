#include "framework/network/connection.h"
#include "framework/network/client.h"

Client::Client(TCPsocket socket, Connection* connection)
{
    m_socket = socket;
    m_connection = connection;
}

Client::~Client()
{
}

bool Client::onReceiveMessage(int32_t size)
{
    LOGI("received message: %i", size);
    return true;
}

Buffer* Client::getInputBuffer() const
{
    return m_connection->getInputBuffer();
}

Buffer* Client::getOutputBuffer() const
{
    return m_connection->getOutputBuffer();
}
