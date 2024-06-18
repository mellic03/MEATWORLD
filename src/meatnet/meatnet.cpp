#include "meatnet.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>

#include <libidk/idk_log.hpp>



Meatnet::Meatnet()
{
    int res = SDLNet_Init();

    if (res != 0)
    {
        std::cout << SDLNet_GetError() << "\n";
    }

    else
    {
        std::cout << "[Meatnet::Meatnet] SDLNet Initialized" << "\n";
    }

    m_positions.resize(4*NUM_PLAYERS);
    for (int i=0; i<NUM_PLAYERS; i++)
    {
        m_positions[i].x = 2.0f * float(i);
        m_positions[i].y = 0.0f;
        m_positions[i].z = 0.0f;
    }

    m_running.store(true);
    m_shutdown.store(false);
}



void
Meatnet::update( idk::EngineAPI &api, std::vector<int> &players )
{

}


void
Meatnet::writeData( const std::vector<glm::vec3> &data )
{
    // m_mutex.lock();
    // std::memcpy(m_positions.data(), data.data(), NUM_PLAYERS*sizeof(glm::vec3));
    // m_mutex.unlock();
}


void
Meatnet::readData( std::vector<glm::vec3> &data )
{
    // m_mutex.lock();
    // data.resize(NUM_PLAYERS);
    // std::memcpy(data.data(), m_positions.data(), NUM_PLAYERS*sizeof(glm::vec3));
    // m_mutex.unlock();
}



void
Meatnet::writePosition( int idx, const glm::vec3 &position )
{
    std::unique_lock lock(m_mutex);
    m_positions[idx] = position;
    lock.unlock();
}


glm::vec3
Meatnet::readPosition( int idx )
{
    std::unique_lock lock(m_mutex);
    glm::vec3 position = m_positions[idx];
    lock.unlock();

    return position;
}




void
Meatnet::shutdown()
{
    while (running())
    {
        m_shutdown.store(true);
    }
}


void
Meatnet::server_init( uint16_t port )
{
    m_status = 1;
    std::thread thread(&Meatnet::_server_thread, this, port);
    thread.detach();
}



void
Meatnet::client_init( const std::string &host, uint16_t port )
{
    m_status = 2;
    m_host = host;    
    std::thread thread(&Meatnet::_client_thread, this, port);
    thread.detach();
}



void
Meatnet::_client_thread( uint16_t port )
{
    std::cout << "Meatnet::_client_thread" << "\n";

    UDPsocket socket = SDLNet_UDP_Open(0);
    IPaddress address;

    if (SDLNet_ResolveHost(&address, m_host.c_str(), port) == -1)
    {
        std::cout << "Error on SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        return;
    }

    /* bind server address to channel 0 */
    int channel = SDLNet_UDP_Bind(socket, -1, &address);
    if (channel == -1)
    {
        std::cout << "Error on SDLNet_UDP_Bind: " << SDLNet_GetError() << "\n";
        return;
    }

    UDPpacket *packet = SDLNet_AllocPacket(Meatnet::NUM_PLAYERS*sizeof(glm::vec3));
    
    packet->address.host = address.host;
    packet->address.port = address.port;
    packet->len = Meatnet::NUM_PLAYERS*sizeof(glm::vec3);


    while (running())
    {
        if (m_shutdown.load() == true)
        {
            m_running.store(false);
        }

        std::unique_lock lock(m_mutex);
        std::memcpy(packet->data, m_positions.data(), Meatnet::NUM_PLAYERS*sizeof(glm::vec3));
        lock.unlock();

        int sent = SDLNet_UDP_Send(socket, channel, packet);

        if (sent == 0)
        {
            std::cout << "Error on SDLNet_UDP_Send: " << SDLNet_GetError() << "\n";
        }
    }


    SDLNet_UDP_Close(socket);
    SDLNet_FreePacket(packet);
}



void
Meatnet::_server_thread( uint16_t port )
{
    std::cout << "Meatnet::_server_thread" << "\n";

    UDPsocket socket = SDLNet_UDP_Open(port);

    if (!socket)
    {
        std::cout << "Error on SDLNet_UDP_Open: " << SDLNet_GetError() << "\n";
        return;
    }


    IPaddress address;
    if (SDLNet_ResolveHost(&address, NULL, port) == -1)
    {
        std::cout << "Error on SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        return;
    }


    UDPpacket *packet = SDLNet_AllocPacket(Meatnet::NUM_PLAYERS*sizeof(glm::vec3));

    while (running())
    {
        if (m_shutdown.load() == true)
        {
            m_running.store(false);
        }


        int recieved = SDLNet_UDP_Recv(socket, packet);

        if (recieved == -1)
        {
            std::cout << "Error on SDLNet_UDP_Recv: " << SDLNet_GetError() << "\n";
        }
    
        else if (recieved == 0)
        {
            // std::cout << "No packets" << "\n";
        }

        else if (recieved > 0)
        {
            std::cout << "[Meatnet::server] Recieved player positions" << "\n";

            if (packet->len < Meatnet::NUM_PLAYERS*sizeof(glm::vec3))
            {
                std::cout << "[Meatnet::server] Malformed Packet" << "\n";
                continue;
            }

            std::unique_lock lock(m_mutex);
            std::memcpy(m_positions.data(), packet->data, Meatnet::NUM_PLAYERS*sizeof(glm::vec3));
            std::cout << "Received position: " << m_positions[1].x << ", " << m_positions[1].z << "\n";
            lock.unlock();

        }

    }


    SDLNet_UDP_Close(socket);
    SDLNet_FreePacket(packet);

}


