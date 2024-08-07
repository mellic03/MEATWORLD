#include "meatnet.hpp"



void
meatnet::Client::send( const PeerData &data )
{
    {
        lock_type lock(m_send_mutex);

        // m_idx = (m_idx + 1) % 8;

        std::memcpy(&m_playerdata[0], &data, sizeof(PeerData));
        m_playerdata[m_idx].timestamp = getTime();

    }

}


bool
meatnet::Client::recv( PeerData *data )
{
    {
        lock_type lock(m_recv_mutex);
        std::memcpy(data, &m_peers, 4*sizeof(PeerData));
    }

    return true;
}



meatnet::Client::Client( const std::string &username, const std::string &hostname,
                         uint16_t port, std::function<void(std::string)> callback )
:   m_username (username),
    m_hostname (hostname),
    m_callback (callback)
{
    UDPsocket socket;

    if (SDLNet_Init() < 0)
    {
        printf("Couldn't initialize net: %s\n", SDLNet_GetError());
        exit(1);
    }

    socket = SDLNet_UDP_Open(0);
    if(!socket)
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }


    std::thread thread1(&meatnet::Client::_in_thread,  this, socket);
    std::thread thread2(&meatnet::Client::_out_thread, this, socket, m_hostname.c_str());

    thread1.detach();
    thread2.detach();
}




void
meatnet::Client::_in_thread( UDPsocket socket )
{
    UDPpacket *packet = SDLNet_AllocPacket(sizeof(PeerData));

    if (!packet)
    {
        printf("Could not allocate packet\n");
        exit(2);
    }


    PeerData buffer;

    while (running())
    {
        if (SDLNet_UDP_Recv(socket, packet))
        {
            std::memcpy(&buffer, packet->data, sizeof(PeerData));

            {
                lock_type lock(m_recv_mutex);

                if (m_peers[buffer.id].timestamp <= buffer.timestamp)
                {
                    m_peers[buffer.id] = buffer;
                }
            }
        }

    }
}


void
meatnet::Client::_out_thread( UDPsocket socket, const char *host )
{
    IPaddress  serverIP;
    UDPpacket* packet;
    SDLNet_SocketSet socketset = NULL;
    int numused;
    static const int MAX_PACKET_SIZE = sizeof(PeerData);


    if (SDLNet_ResolveHost(&serverIP, host, 4201) == -1)
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(2);
    }

    packet = SDLNet_AllocPacket(MAX_PACKET_SIZE);

    if (!packet)
    {
        printf("Could not allocate packet\n");
        exit(2);
    }


    PeerData buffer;

    while (running())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        // for (int i=0; i<8; i++)
        {
            {
                lock_type lock(m_send_mutex);
                std::memcpy(packet->data, &m_playerdata[0], sizeof(PeerData));
            }

            packet->address.host = serverIP.host;
            packet->address.port = serverIP.port;
            packet->len = sizeof(PeerData);

            SDLNet_UDP_Send(socket, -1, packet);
        }

    }
}




// TCPsocket
// meatnet::Client::_connect( const char *host, uint16_t port )
// {
//     IPaddress address;

//     if (SDLNet_ResolveHost(&address, host, port) == -1)
//     {
//         return NULL;
//     }

//     return SDLNet_TCP_Open(&address);
// }




// void
// meatnet::Client::_msg_thread( const char *host, uint16_t port )
// {
//     TCPsocket socket = _connect(host, port);

//     if (socket == NULL)
//     {
//         std::cout << "[meatnet::Client] Error connecting to " << host << "::" << port << "\n";
//         shutdown();
//         return;
//     }


//     Message msg;
//     void *data;

//     while (running())
//     {
//         if (msg_recv(socket, msg, data))
//         {

//         }

//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }

// }



// bool
// meatnet::Client::_make_request( TCPsocket server, uint32_t req, meatnet::Message &msg, void *buffer )
// {
//     msg.timestamp = uint32_t(std::time(NULL));
//     msg.request   = req;
//     msg.response  = RES_NONE;

//     if (!msg_send(server, msg, buffer))
//     {
//         // std::cout << "[meatnet::Client] Making " << req_str(msg.request) << " request... failure\n";
//         return false;
//     }

//     // std::cout << "[meatnet::Client] Making " << req_str(msg.request) << " request... (2)\n";

//     if (!msg_recv(server, msg, buffer))
//     {
//         // std::cout << "[meatnet::Client] Making " << req_str(msg.request) << " request... failure\n";
//         return false;
//     }

//     if (msg.response == RES_REFUSE)
//     {
//         std::cout << "[meatnet::Client] Request refused, reason: \"" << msg.header << "\"\n";
//     }

//     return true;
// }




// void
// meatnet::Client::_process_user_requests( TCPsocket server, void *buffer )
// {
//     {
//         lock_type lock(m_mutex);

//         IDK_ASSERT("Ruh roh", m_user_requests.size() == m_user_callbacks.size());

//         if (m_user_requests.empty())
//         {
//             return;
//         }
//     }


//     SyncBufferRequest request;

//     {
//         lock_type lock(m_mutex);

//         if (m_user_requests.size() > 0)
//         {
//             request = m_user_requests.front();
//                       m_user_requests.pop();
//         }
//     }


//     std::memcpy(buffer, &request, sizeof(SyncBufferRequest));

//     Message msg(REQ_CREATE_BUF, sizeof(SyncBufferRequest));

//     if (!_make_request(server, REQ_CREATE_BUF, msg, buffer))
//     {
//         std::cout << "Ruh roh" << std::endl;
//         exit(1);
//     }

//     // Call user callback on success
//     {
//         lock_type lock(m_mutex);

//         m_sync_buffers[request.name] = {
//             .stride = request.stride,
//             .nbytes = request.nbytes,
//             .data   = std::malloc(request.nbytes)
//         };

//         m_user_callbacks.front()();
//         m_user_callbacks.pop();
//     }

// }





// void
// meatnet::Client::_main_thread( const char *host, uint16_t port )
// {
//     TCPsocket server = _connect(host, port);

//     if (server == NULL)
//     {
//         std::cout << "[meatnet::Client] Error connecting to " << host << "::" << port << "\n";
//         SDLNet_TCP_Close(server);
//         shutdown();
//         return;
//     }


//     Message msg;
//     void *buffer = std::malloc(BUFFER_SIZE);
//     std::memset(buffer, '\0', BUFFER_SIZE);

//     // Request player ID
//     {
//         if (!_make_request(server, REQ_JOIN, msg, buffer))
//         {
//             _error(server);
//         }

//         m_clientID = ((int *)buffer)[0];
//         std::cout << "[meatnet::Client] Connected to server as client " << m_clientID << "\n";
//     }


//     // Download map file
//     {
//         if (!_make_request(server, REQ_FILE, msg, buffer))
//         {
//             _error(server);
//             SDLNet_TCP_Close(server);
//             shutdown();
//             return;
//         }

//         File    *file = (File *)(buffer);
//         uint8_t *data = (uint8_t *)(buffer) + sizeof(File);

//         std::cout
//             << "[meatnet::Client] Recieved "
//             << file->nbytes << " byte map file "
//             << "\"" << file->name << "\""
//             << "from server\n";

//         m_filepath = "./meatnet-data/" + std::string(file->name);

//         std::ofstream stream(m_filepath, std::ios::binary);
//         stream.write(reinterpret_cast<const char *>(data), file->nbytes);
//         stream.close();

//         _set_status(STATUS_CONNECTED);
//     }


//     while (running())
//     {
//         _process_user_requests(server, buffer);

//         for (auto &[name, sb]: m_sync_buffers)
//         {
//             if (sb.data == nullptr)
//             {
//                 continue;
//             }


//             // Request data of other players
//             // -----------------------------------------------------------------------------
//             msg = Message(REQ_BUF_GET, 0);

//             if (!_make_request(server, REQ_BUF_GET, msg, buffer))
//             {
//                 std::cout << "Ruh roh" << std::endl;
//                 exit(1);
//             }

//             {
//                 lock_type lock(m_mutex);
//                 std::memcpy(sb.data, buffer, sb.nbytes);
//             }
//             // -----------------------------------------------------------------------------


//             // Send data of this player
//             // -----------------------------------------------------------------------------
//             msg = Message(REQ_BUF_SUBDATA, sb.stride, name);

//             uint8_t *src = (uint8_t *)(sb.data);
//                      src = src + m_clientID * sb.stride;


//             {
//                 lock_type lock(m_mutex);
//                 std::memcpy(buffer, src, sb.stride);
//             }

//             if (!_make_request(server, REQ_BUF_SUBDATA, msg, buffer))
//             {
//                 std::cout << "Ruh roh" << std::endl;
//                 exit(1);
//             }
//             // -----------------------------------------------------------------------------

//         }


//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }


//     SDLNet_TCP_Close(server);
// }






// bool
// meatnet::Client::readSyncBuffer( const std::string &name, void *buffer )
// {
//     lock_type lock(m_mutex);

//     if (m_sync_buffers.contains(name) == false)
//     {
//         return false;
//     }

//     SyncBuffer &sb = m_sync_buffers[name];

//     if (sb.data == nullptr)
//     {
//         return false;
//     }

//     std::memcpy(buffer, sb.data, sb.nbytes);

//     return true;
// }
