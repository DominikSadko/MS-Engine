#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "framework/core/platform.h"
#include <stdex/buffer.h>

class Client;

class Connection
{
    public:
        Connection(uint8_t id, uint16_t port, const std::string& host = "");
        ~Connection();

        void init();
        void terminate();

        void loop();
        static int32_t onThread(void* ptr);

        uint8_t getId() const { return m_id; }
        TCPsocket getSocket() const { return m_socket; }
        uint16_t getPort() const { return m_port; }
        const std::string& getHost() const { return m_host; }

        Buffer* getInputBuffer() const { return m_inputBuffer; }
        Buffer* getOutputBuffer() const { return m_outputBuffer; }

        bool isConnected() const { return m_socket; }

        void disconnectClient(Client* client);

    public:
        uint8_t m_id;
        SDL_Thread* m_thread;
        TCPsocket m_socket;
        SDLNet_SocketSet m_socketSet;

        uint16_t m_port;
        std::string m_host;

        Buffer* m_inputBuffer;
        Buffer* m_outputBuffer;

        std::vector<Client*> m_clients;
};

#endif // __CONNECTION_H__ //
