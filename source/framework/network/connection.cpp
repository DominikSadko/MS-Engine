#include "framework/network/client.h"
#include "framework/network/connection.h"
#include <stdex/format.h>

#define NETWORK_BUFFER_SIZE 65535

Connection::Connection(uint8_t id, uint16_t port, const std::string& host)
{
    m_id = id;
    m_port = port;
    m_host = host;

    m_inputBuffer = new stdex::buffer(NETWORK_BUFFER_SIZE);
    m_outputBuffer = new stdex::buffer(NETWORK_BUFFER_SIZE);
}

Connection::~Connection()
{
    terminate();

    delete m_inputBuffer;
    delete m_outputBuffer;
}

void Connection::terminate()
{
    for(auto client : m_clients)
        delete client;

    m_clients.clear();

    SDL_DetachThread(m_thread);
    SDLNet_TCP_Close(m_socket);
    SDLNet_FreeSocketSet(m_socketSet);

    m_thread = NULL;
    m_socket = 0;
}

void Connection::init()
{
    IPaddress ip;

    if(SDLNet_ResolveHost(&ip, m_host.empty() ? NULL : m_host.c_str(), m_port) < 0)
        LOGE("SDLNet_ResolveHost: %s", SDLNet_GetError());

    if(!(m_socket = SDLNet_TCP_Open(&ip)))
        LOGE("SDLNet_TCP_Open: %s", SDLNet_GetError());

    if(!(m_socketSet = SDLNet_AllocSocketSet(0xFFFF))) // todo max connections
        LOGE("SDLNet_AllocSocketSet: %s", SDLNet_GetError());

    SDLNet_TCP_AddSocket(m_socketSet, m_socket);
    m_thread = SDL_CreateThread(Connection::onThread, stdex::toString("Connection_", m_id).c_str(), (void*)this);
}

void Connection::loop()
{
    TCPsocket socket;

    while(m_socket)
    {
        if(!SDLNet_CheckSockets(m_socketSet, 0))
            continue;

        if(SDLNet_SocketReady(m_socket))
        {
            if((socket = SDLNet_TCP_Accept(m_socket)))
            {
                Client* client = new Client(socket, this);
                SDLNet_TCP_AddSocket(m_socketSet, socket);

                m_clients.push_back(client);
            }
            else
            {
                terminate();
                break;
            }
        }

        for(auto client : m_clients)
        {
            if(SDLNet_SocketReady(client->getSocket()))
            {
                int32_t size = SDLNet_TCP_Recv(client->getSocket(), m_inputBuffer->getBuffer(), m_inputBuffer->getSize());
                if(size <= 0 || !client->onReceiveMessage(size))
                {
                    // todo timeout
                    disconnectClient(client);
                    break;
                }
            }
        }
    }
}

int32_t Connection::onThread(void* ptr)
{
    Connection* connection = (Connection*)(ptr);
    connection->loop();

    return 0;
}

void Connection::disconnectClient(Client* client)
{
    auto it = std::find(m_clients.begin(), m_clients.end(), client);
    if(it != m_clients.end())
        m_clients.erase(it);

    SDLNet_TCP_DelSocket(m_socketSet, client->getSocket());
    SDLNet_TCP_Close(client->getSocket());
    delete client;
}
