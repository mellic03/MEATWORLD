#include "meatnet.hpp"
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>



void
meatnet::Host::update( idk::EngineAPI &api, std::vector<int> &players )
{


}



void
meatnet::Host::connect( const std::string &filepath, uint16_t port )
{
    m_filepath = filepath;

    std::cout << "[meatnet::Host] Opening server on port " << port << std::endl;

    m_players.resize(MAX_PLAYERS);
    for (int i=0; i<MAX_PLAYERS; i++)
    {
        m_players[i].player_id = -1;
        // m_players[i].position = glm::vec3(0.0f, 0.75f, 2.0f*i);
    }

    _set_status(STATUS_CONNECTING);

    std::thread thread1(&meatnet::Host::_msg_thread,  this, PORT_MAIN);
    std::thread thread2(&meatnet::Host::_main_thread, this, PORT_MAIN);

    thread1.detach();
    thread2.detach();
}



TCPsocket
meatnet::Host::_client_join( TCPsocket server, int idx )
{
    using namespace meatnet;

    TCPsocket client = SDLNet_TCP_Accept(server);

    if (!client)
    {
        return NULL;
    }


    ClientBuffer buffer = ClientBuffer_new(-1);

    while (running())
    {
        int received = SDLNet_TCP_Recv(client, (void *)(&buffer), sizeof(ClientBuffer));

        if (received)
        {
            std::cout << "[Host::_client_join] Request: " << buffer.header << ": " << buffer.body << "\n";
            break;
        }
    }

    buffer.response = idx;

    while (running())
    {
        int sent = SDLNet_TCP_Send(client, (const void *)(&buffer), sizeof(ClientBuffer));

        if (sent)
        {
            break;
        }
    }

    file_send(client, m_filepath);

    return client;
}



void
meatnet::Host::_send_players( TCPsocket socket, void *data )
{
    while (running())
    {
        int sent = SDLNet_TCP_Send(socket, data, 4*sizeof(PlayerBuffer));

        if (sent == 4*sizeof(PlayerBuffer))
        {
            break;
        }
    }
}



void
meatnet::Host::_msg_thread( uint16_t port )
{
    int idx = 0;

    while (running())
    {
        idx = (idx + 1) % MAX_PLAYERS;
    }
}



void
meatnet::Host::_main_thread( uint16_t port )
{
    static std::vector<TCPsocket> clients(MAX_PLAYERS, NULL);
    static std::vector<bool>      connected(MAX_PLAYERS, false);


    IPaddress server_address;

    if (SDLNet_ResolveHost(&server_address, NULL, PORT_HANDSHAKE) == -1)
    {
        std::cout << "[Host::handshake] Error on SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        _set_status(STATUS_ERROR);
        return;
    }


    TCPsocket server_socket = SDLNet_TCP_Open(&server_address);

    if (!server_socket)
    {
        std::cout << "[Host::handshake] Error on SDLNet_TCP_Open: "
                  << SDLNet_GetError() << "\n";

        _set_status(STATUS_ERROR);
        return;
    }



    ClientBuffer buffer;
    std::vector<PlayerBuffer> players(4);

    int idx = 0;

    while (running())
    {
        if (clients[idx] == NULL)
        {
            TCPsocket client = _client_join(server_socket, idx);
            clients[idx] = client;
            m_players[idx].player_id = idx;
        }

        else
        {
            int received = SDLNet_TCP_Recv(clients[idx], (void *)(&buffer), sizeof(ClientBuffer));

            if (received == sizeof(ClientBuffer))
            {
                std::memcpy(&(m_players[idx]), &buffer, sizeof(ClientBuffer));

                players[idx].position = m_players[idx].position;
                players[idx].yaw      = m_players[idx].yaw;
                players[idx].action   = m_players[idx].action;

                _send_players(clients[idx], players.data());
            }
        }

        idx = (idx + 1) % MAX_PLAYERS;
    }


    for (int i=0; i<MAX_PLAYERS; i++)
    {
        if (clients[i])
        {
            SDLNet_TCP_Close(clients[i]);
        }
    }


    SDLNet_TCP_Close(server_socket);
}


