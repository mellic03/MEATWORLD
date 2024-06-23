#include "meatnet.hpp"




meatnet::Host::Host( uint16_t main_port, uint16_t msg_port )
{
    m_playerdata.resize(MAX_PLAYERS);

    m_clients  = std::vector<TCPsocket>(MAX_PLAYERS, NULL);
    m_timers   = std::vector<uint32_t>(MAX_PLAYERS, std::time(NULL) - 60);

    std::thread thread1(&meatnet::Host::_main_thread, this, main_port);
    // std::thread thread2(&meatnet::Host::_msg_thread,  this, msg_port);

    thread1.detach();
    // thread2.detach();
}


void
meatnet::Host::loadScene( const std::string &filepath )
{
    std::cout << "[meatnet::Host::loadScene]\n";
    m_filepath = filepath;
}





TCPsocket
meatnet::Host::_open_connection( uint16_t port )
{
    IPaddress address;

    if (SDLNet_ResolveHost(&address, NULL, port) == -1)
    {
        return NULL;
    }

    return SDLNet_TCP_Open(&address);;
}



TCPsocket
meatnet::Host::_client_connect( TCPsocket server )
{
    TCPsocket client = SDLNet_TCP_Accept(server);

    if (!client)
    {
        return NULL;
    }

    return client;
}



void
meatnet::Host::_msg_thread( uint16_t port )
{
    // TCPsocket socket = _open_connection(port);

    // if (!socket)
    // {
    //     shutdown();
    //     return;
    // }


    // Header header;
    // void *data = std::malloc(BUFFER_SIZE);
    // std::memset(data, '\0', BUFFER_SIZE);

    // int idx = 0;

    // while (running())
    // {
    //     idx = (idx + 1) % MAX_PLAYERS;

    // }

}



void
meatnet::Host::_process_req_disconnect( int idx, meatnet::Header &header, void *buffer )
{
    std::cout << "[meatnet::Host] Client disconnected\n";
}



void
meatnet::Host::_process_req_player( int idx, meatnet::Header &header, void *buffer )
{
    {
        lock_type lock(m_mutex);
        std::memcpy(&(m_playerdata[idx]), buffer, sizeof(PlayerData));
        std::memcpy(buffer, m_playerdata.data(), MAX_PLAYERS*sizeof(PlayerData));
    }

    header.response = RES_PEERDATA;
    header.nbytes   = MAX_PLAYERS * sizeof(PlayerData);
}



void
meatnet::Host::_process_req_file( int idx, meatnet::Header &header, void *buffer )
{
    namespace fs = std::filesystem;

    size_t file_nbytes;

    std::ifstream stream(m_filepath, std::ios::binary);
    stream.seekg(0, std::ios::end);
    file_nbytes = stream.tellg();
    stream.seekg(0, std::ios::beg);



    meatnet::File file;

    std::strcpy(file.name, fs::path(m_filepath).filename().c_str());
    file.nbytes = file_nbytes;

    std::memcpy(buffer, (const void *)(&file), sizeof(File));
    stream.read(reinterpret_cast<char *>((uint8_t *)(buffer) + sizeof(File)), file.nbytes);
    stream.close();


    header.response = RES_FILE;
    header.nbytes = sizeof(File) + file.nbytes;
}



void
meatnet::Host::_process_request( int idx, meatnet::Header &header, void *buffer )
{
    std::cout << "[meatnet::Host] Processing " << req_str(header.request) << " request... (1)\n";

    header.response = RES_NONE;

    if (!data_recv(m_clients[idx], header, buffer))
    {
        std::cout << "rip\n";
        return;
    }

    switch (header.request)
    {
        default:
            std::cout << "[meatnet::Host] Invalid request\n";
            break;
    
        case REQ_DISCONNECT:
            _process_req_disconnect(idx, header, buffer);
            break;

        case REQ_FILE:
            _process_req_file(idx, header, buffer);
            break;

        case REQ_PLAYERDATA:
            _process_req_player(idx, header, buffer);
            break;
    }


    header.timestamp = uint32_t(std::time(NULL));

    if (!msg_send(m_clients[idx], header, buffer))
    {
        std::cout << "rip\n";
        return;
    }

    std::cout << "[meatnet::Host] Processing " << req_str(header.request) << " request... success\n";

}



void
meatnet::Host::_main_thread( uint16_t port )
{
    TCPsocket server = _open_connection(port);

    if (!server)
    {
        std::cout << "[meatnet::Host] Error opening connection on port " << port << "\n";
        shutdown();
        return;
    }
    std::cout << "[meatnet::Host] Connection opened on port " << port << "\n";




    Header header;
    void *buffer = std::malloc(BUFFER_SIZE);
    std::memset(buffer, '\0', BUFFER_SIZE);


    int idx = 0;
    uint32_t time_now, time_delta;

    while (running())
    {
        idx = (idx + 1) % MAX_PLAYERS;

        if (m_clients[idx] == NULL)
        {
            TCPsocket client = _client_connect(server);
        
            if (client)
            {
                m_clients[idx] = client;
                m_timers[idx]  = uint32_t(std::time(NULL));
            }
            
            continue;
        }


        time_now = uint32_t(std::time(NULL));

        if (header_recv(m_clients[idx], header, buffer))
        {
            m_timers[idx] = time_now;
            _process_request(idx, header, buffer);
        }

        else if (time_now - m_timers[idx] > TIMEOUT_SECONDS)
        {
            SDLNet_TCP_Close(m_clients[idx]);
            m_clients[idx] = NULL;
            std::cout << "[meatnet::Host] Disconnected client " << idx << " (timeout)\n";
        }

    }


    for (int i=0; i<MAX_PLAYERS; i++)
    {
        if (m_clients[i])
        {
            SDLNet_TCP_Close(m_clients[i]);
        }
    }


    SDLNet_TCP_Close(server);
}


