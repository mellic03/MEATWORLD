#include "meatnet.hpp"
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKEvents/IDKEvents.hpp>


void
meatnet::Client::_write_data( const glm::vec3 &pos, float yaw, uint32_t action )
{
    std::unique_lock lock(m_mutex);

    m_buffer.position = pos;
    m_buffer.yaw      = yaw;
    m_buffer.action   = action;

    lock.unlock();
}



void
meatnet::Client::update( idk::EngineAPI &api, int player, std::vector<int> &players )
{
    using namespace idk;


    uint32_t status = _get_status();

    if (status == STATUS_CONNECTED)
    {
        m_callback(m_filepath);
        _set_status(STATUS_GAMEPLAY);
        return;
    }

    else if (status != STATUS_GAMEPLAY)
    {
        return;
    }

    auto &ren = api.getRenderer();

    // uint32_t A = 0;

    // if (api.getEventSys().mouseDown(MouseButton::LEFT))
    // {
    //     A = 1;
    // }

    // _write_data(
    //     TransformSys::getPositionWorldspace(player),
    //     TransformSys::getData(player).yaw,
    //     A
    // );


    // std::unique_lock lock(m_mutex);

    // std::vector<int>       ids      (m_players.size(), -1);
    // std::vector<glm::vec3> position (m_players.size());
    // std::vector<float>     yaw      (m_players.size());
    // std::vector<uint32_t>  action   (m_players.size());

    // for (int i=0; i<MAX_PLAYERS; i++)
    // {
    //     if (m_players[i].player_id == -1)
    //     {
    //         continue;
    //     }

    //     ids[i]      = m_players[i].player_id;
    //     position[i] = m_players[i].position;
    //     yaw[i]      = m_players[i].yaw;
    //     action[i]   = m_players[i].action;
    // }

    // lock.unlock();


    // for (int i=0; i<MAX_PLAYERS; i++)
    // {
    //     if (ids[i] == -1)
    //     {
    //         continue;
    //     }

    //     if (i == m_player_id.load())
    //     {
    //         continue;
    //     }

    //     // idk_printvalue(ids[i]);
    //     // std::cout << "Position " << i << ": " << position[i].x << ", " << position[i].z << "\n\n";

    //     glm::vec3 prev = TransformSys::getPositionWorldspace(players[i]);
    //     glm::vec3 delta = position[i] - prev;

    //     TransformSys::translateWorldspace(players[i], delta);
    //     TransformSys::getData(players[i]).yaw = yaw[i];
    // }
    
}



void
meatnet::Client::connect( const std::string &name, const char *host, uint16_t port,
                           std::function<void(std::string)> callback )
{
    m_callback = callback;

    m_name = name;
    m_player_id.store(-1);

    m_players.resize(MAX_PLAYERS);
    for (int i=0; i<MAX_PLAYERS; i++)
    {
        m_players[i].player_id = -1;
    }

    _set_status(STATUS_CONNECTING);
    std::thread thread(&meatnet::Client::_client_thread, this, host, port);
    thread.detach();
}



int
meatnet::Client::_handshake( const char *host )
{
    return 0;
}



void
meatnet::Client::_recv_players( TCPsocket socket )
{
    std::vector<PlayerBuffer> players(4);

    while (running())
    {
        int received = SDLNet_TCP_Recv(socket, players.data(), 4*sizeof(PlayerBuffer));

        if (received == 4*sizeof(PlayerBuffer))
        {
            std::unique_lock lock(m_mutex);
            std::memcpy(m_players.data(), players.data(), 4*sizeof(PlayerBuffer));
            lock.unlock();

            break;
        }
    }
}




void
meatnet::Client::_client_thread( const char *host, uint16_t port )
{
    IPaddress address;

    if (SDLNet_ResolveHost(&address, host, PORT_HANDSHAKE) == -1)
    {
        std::cout << "[Client::handshake] Error on SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        _set_status(STATUS_ERROR);
        return;
    }

    TCPsocket socket = SDLNet_TCP_Open(&address);


    {
        std::unique_lock lock(m_mutex);

        std::strcpy(m_buffer.header, "MEATJOIN");
        std::strcpy(m_buffer.body,   m_name.c_str());

        while (running())
        {
            int sent = SDLNet_TCP_Send(socket, (const void *)(&m_buffer), sizeof(ClientBuffer));

            if (sent == sizeof(ClientBuffer))
            {
                break;
            }
        }


        while (running())
        {
            int received = SDLNet_TCP_Recv(socket, (void *)(&m_buffer), sizeof(ClientBuffer));

            if (received == sizeof(ClientBuffer))
            {
                break;
            }
        }

        std::cout << "[Client::handshake] Joined as player " << m_buffer.response << "\n";

        m_player_id.store(m_buffer.response);

        lock.unlock();
    }


    // Receive .idksc map file from server
    {
        meatnet::File file;
        file_recv(socket, file);

        std::filesystem::create_directory("./temp_scenes");

        m_filepath = "./temp_scenes/" + file.name;
        std::cout << "Writing " << file.nbytes << " bytes to " << m_filepath << std::endl;
        std::ofstream stream(m_filepath, std::ios::binary);
        stream.write(reinterpret_cast<const char *>(file.data), file.nbytes);
        stream.close();

        _set_status(STATUS_CONNECTED);
    }




    ClientBuffer buffer;

    while (running())
    {
        std::unique_lock lock(m_mutex);
        std::memcpy(&buffer, &m_buffer, sizeof(ClientBuffer));
        lock.unlock();

        int sent = SDLNet_TCP_Send(socket, (const void *)(&buffer), sizeof(ClientBuffer));

        if (sent == sizeof(ClientBuffer))
        {
            _recv_players(socket);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    SDLNet_TCP_Close(socket);

}

