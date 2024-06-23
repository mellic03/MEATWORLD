#include "meatnet.hpp"

#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKEvents/IDKEvents.hpp>



std::vector<meatnet::PlayerData>
meatnet::Client::getPeers()
{
    using namespace idk;

    std::vector<PlayerData> peers(MAX_PLAYERS);

    // Copy data
    {
        lock_type lock(m_mutex);
        std::memcpy(peers.data(), m_peers.data(), MAX_PLAYERS*sizeof(PlayerData));
    }

    return peers;
}


void
meatnet::Client::update( meatnet::PlayerData &player )
{
    using namespace idk;

    uint32_t status = _get_status();

    if (status == STATUS_CONNECTED)
    {
        m_callback(m_filepath);
        _set_status(STATUS_GAMEPLAY);
    }

    {
        lock_type lock(m_mutex);
        std::memcpy(&(m_playerdata), &(player), sizeof(PlayerData));
    }
}



meatnet::Client::Client( const std::string &username, const std::string &hostname,
                         uint16_t port, std::function<void(std::string)> callback )
:   m_username (username),
    m_hostname (hostname),
    m_callback (callback),
    m_peers    (MAX_PLAYERS)
{
    _set_status(STATUS_CONNECTING);

    std::thread thread1(&meatnet::Client::_main_thread, this, m_hostname.c_str(), port);
    // std::thread thread2(&meatnet::Client::_msg_thread,  this, m_hostname.c_str(), port+1);

    thread1.detach();
    // thread2.detach();
}





TCPsocket
meatnet::Client::_connect( const char *host, uint16_t port )
{
    IPaddress address;

    if (SDLNet_ResolveHost(&address, host, port) == -1)
    {
        return NULL;
    }

    return SDLNet_TCP_Open(&address);
}



void
meatnet::Client::_msg_thread( const char *host, uint16_t port )
{
    TCPsocket socket = _connect(host, port);

    if (socket == NULL)
    {
        std::cout << "[meatnet::Client] Error connecting to " << host << "::" << port << "\n";
        shutdown();
        return;
    }


    Header header;
    void *data;

    while (running())
    {
        if (msg_recv(socket, header, data))
        {

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}




bool
meatnet::Client::_make_request( TCPsocket server, uint32_t req, Header &header, void *buffer )
{
    header.timestamp = uint32_t(std::time(NULL));
    header.request   = req;
    header.response  = RES_NONE;

    // std::cout << "[meatnet::Client] Making " << req_str(header.request) << " request... (1)\n";
    // std::cout << "[meatnet::Client] header.nbytes: " << header.nbytes << "\n";

    if (!msg_send(server, header, buffer))
    {
        // std::cout << "[meatnet::Client] Making " << req_str(header.request) << " request... failure\n";
        return false;
    }

    // std::cout << "[meatnet::Client] Making " << req_str(header.request) << " request... (2)\n";

    if (!msg_recv(server, header, buffer))
    {
        // std::cout << "[meatnet::Client] Making " << req_str(header.request) << " request... failure\n";
        return false;
    }

    // std::cout << "[meatnet::Client] Making " << req_str(header.request) << " request... success\n";

    return true;
}




void
meatnet::Client::_main_thread( const char *host, uint16_t port )
{
    TCPsocket server = _connect(host, port);

    if (server == NULL)
    {
        std::cout << "[meatnet::Client] Error connecting to " << host << "::" << port << "\n";
        SDLNet_TCP_Close(server);
        shutdown();
        return;
    }


    Header header;
    void *buffer = std::malloc(BUFFER_SIZE);
    std::memset(buffer, '\0', BUFFER_SIZE);

    // Download map file
    {
        if (!_make_request(server, REQ_FILE, header, buffer))
        {
            SDLNet_TCP_Close(server);
            shutdown();
            return;
        }

        File    *file = (File *)(buffer);
        uint8_t *data = (uint8_t *)(buffer) + sizeof(File);

        std::cout
            << "[meatnet::Client] Recieved "
            << file->nbytes << " byte map file "
            << "\"" << file->name << "\""
            << "from server\n";

        m_filepath = "./meatnet-data/" + std::string(file->name);

        std::ofstream stream(m_filepath, std::ios::binary);
        stream.write(reinterpret_cast<const char *>(data), file->nbytes);
        stream.close();

        _set_status(STATUS_CONNECTED);
    }


    while (running())
    {
        {
            lock_type lock(m_mutex);
            std::memcpy(buffer, &m_playerdata, sizeof(PlayerData));
        }

        header.nbytes = sizeof(PlayerData);
        _make_request(server, REQ_PLAYERDATA, header, buffer);

        if (header.response != RES_PEERDATA)
        {
            perror("Ruh roh");
        }

        {
            lock_type lock(m_mutex);
            std::memcpy(m_peers.data(), buffer, MAX_PLAYERS*sizeof(PlayerData));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    SDLNet_TCP_Close(server);

}



