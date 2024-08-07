#include "meatnet.hpp"




meatnet::Host::Host( uint16_t main_port, uint16_t msg_port )
{
    // m_clients  = std::vector<TCPsocket>(MAX_PLAYERS, NULL);
    // m_timers   = std::vector<uint32_t>(MAX_PLAYERS, std::time(NULL) - 60);

    std::memset(m_players, '\0', 4*sizeof(PeerData));
    m_idx.store(0);


    if (SDLNet_Init() < 0)
    {
        printf("Couldn't initialize net: %s\n", SDLNet_GetError());
        exit(1);
    }

    UDPsocket socket = SDLNet_UDP_Open(4201);

    if (!socket)
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }


    std::thread thread1(&meatnet::Host::_in_thread,  this, socket, 4201);
    std::thread thread2(&meatnet::Host::_out_thread, this, socket, 4201);

    thread1.detach();
    thread2.detach();
}


void
meatnet::Host::loadScene( const std::string &filepath )
{
    std::cout << "[meatnet::Host::loadScene]\n";
    m_filepath = filepath;
}



void
meatnet::Host::_out_thread( UDPsocket socket, uint16_t port )
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
        int n = m_idx.load();

        for (int j=0; j<n; j++)
        {
            auto key = m_dst[j];

            packet->address.host = key.first;
            packet->address.port = key.second;
            packet->len = sizeof(PeerData);

            for (int i=0; i<n; i++)
            {
                if (i != j)
                {
                    {
                        lock_type lock(m_recv_mutex);
                        std::memcpy(packet->data, &m_players[i], sizeof(PeerData));
                    }

                    SDLNet_UDP_Send(socket, -1, packet);
                }
            }
        }
    }
}



void
meatnet::Host::_in_thread( UDPsocket socket, uint16_t port )
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
            auto key = std::make_pair(packet->address.host, packet->address.port);

            if (m_clients.contains(key) == false)
            {
                int idx = m_idx.load();
                m_clients[key] = idx;

                {
                    lock_type lock(m_send_mutex);
                    m_dst.push_back(key);
                }

                m_idx.store(idx + 1);
            }

            std::memcpy(&buffer, packet->data, sizeof(PeerData));

            int id = m_clients[key];


            if (m_players[id].timestamp >= buffer.timestamp)
            {
                continue;
            }

            {
                lock_type lock(m_recv_mutex);

                m_players[id] = buffer;
                m_players[id].id = id;
            }
        }


        // for (auto &[key, id]: m_clients)
        // {
        //     packet->address.host = key.first;
        //     packet->address.port = key.second;
        //     packet->len = sizeof(PeerData);

        //     for (int i=0; i<m_idx.load(); i++)
        //     {
        //         if (i != id)
        //         {
        //             std::memcpy(packet->data, &m_players[i], sizeof(PeerData));
        //             SDLNet_UDP_Send(socket, -1, packet);
        //         }
        //     }
        // }
    }
}




// TCPsocket
// meatnet::Host::_open_connection( uint16_t port )
// {
//     IPaddress address;

//     if (SDLNet_ResolveHost(&address, NULL, port) == -1)
//     {
//         return NULL;
//     }

//     return SDLNet_TCP_Open(&address);;
// }



// TCPsocket
// meatnet::Host::_client_connect( TCPsocket server )
// {
//     TCPsocket client = SDLNet_TCP_Accept(server);

//     if (!client)
//     {
//         return NULL;
//     }

//     return client;
// }



// void
// meatnet::Host::_msg_thread( uint16_t port )
// {
//     // TCPsocket socket = _open_connection(port);

//     // if (!socket)
//     // {
//     //     shutdown();
//     //     return;
//     // }


//     // Message header;
//     // void *data = std::malloc(BUFFER_SIZE);
//     // std::memset(data, '\0', BUFFER_SIZE);

//     // int idx = 0;

//     // while (running())
//     // {
//     //     idx = (idx + 1) % MAX_PLAYERS;

//     // }

// }



// void
// meatnet::Host::_req_disconnect( int idx, meatnet::Message &msg, void *buffer )
// {
//     std::cout << "[meatnet::Host] Client disconnected\n";
// }



// void
// meatnet::Host::_req_join( int idx, meatnet::Message &msg, void *buffer )
// {
//     {
//         std::memcpy(buffer, &idx, sizeof(int));
//     }

//     msg.response = RES_OKAY;
//     msg.nbytes   = sizeof(int);
// }



// void
// meatnet::Host::_req_create_buf( int idx, meatnet::Message &msg, void *buffer )
// {
//     if (msg.nbytes != sizeof(SyncBufferRequest))
//     {
//         msg = Message(RES_REFUSE, 0, "Invalid request");
//         return;
//     }

//     SyncBufferRequest *req = (SyncBufferRequest *)(buffer);

//     std::cout
//         << "[meatnet::Host] SyncBufferRequest: "
//         << " name=\""   << req->name << "\", "
//         << " nbytes="   << req->nbytes << "\n";


//     if (m_sync_buffers.contains(req->name))
//     {
//         msg = Message(RES_REFUSE, 0, "Buffer exists");
//         return;
//     }

//     m_sync_buffers[req->name] = {
//         .nbytes = req->nbytes,
//         .data   = std::malloc(req->nbytes)
//     };

//     msg = Message(RES_OKAY, 0);
//     msg.response = RES_OKAY;
// }



// void
// meatnet::Host::_req_buf_get( int idx, meatnet::Message &msg, void *buffer )
// {
//     if (m_sync_buffers.contains(msg.header) == false)
//     {
//         msg = Message(RES_REFUSE, 0, "No such buffer");
//         return;
//     }

//     auto &sb = m_sync_buffers[msg.header];
//     std::memcpy(buffer, sb.data, sb.nbytes);

//     msg = Message(RES_OKAY, 0);

// }


// void
// meatnet::Host::_req_buf_subdata( int idx, meatnet::Message &msg, void *buffer )
// {
//     std::string name = msg.header;

//     if (m_sync_buffers.contains(name) == false)
//     {
//         msg = Message(RES_REFUSE, 0, "No such buffer");
//         return;
//     }


//     size_t stride = m_sync_buffers[name].stride; 
//     void  *dst    = (uint8_t *)(m_sync_buffers[name].data) + idx*stride;

//     std::memcpy(dst, buffer, stride);
//     msg = Message(RES_OKAY, 0);

// }



// void
// meatnet::Host::_req_file( int idx, meatnet::Message &msg, void *buffer )
// {
//     namespace fs = std::filesystem;

//     size_t file_nbytes;

//     std::ifstream stream(m_filepath, std::ios::binary);
//     stream.seekg(0, std::ios::end);
//     file_nbytes = stream.tellg();
//     stream.seekg(0, std::ios::beg);



//     meatnet::File file;

//     std::strcpy(file.name, fs::path(m_filepath).filename().c_str());
//     file.nbytes = file_nbytes;

//     std::memcpy(buffer, (const void *)(&file), sizeof(File));
//     stream.read(reinterpret_cast<char *>((uint8_t *)(buffer) + sizeof(File)), file.nbytes);
//     stream.close();

//     msg.response = RES_FILE;
//     msg.nbytes = sizeof(File) + file.nbytes;
// }



// void
// meatnet::Host::_process_request( int idx, meatnet::Message &msg, void *buffer )
// {
//     msg.response = RES_NONE;

//     if (!data_recv(m_clients[idx], msg, buffer))
//     {
//         std::cout << "rip\n";
//         return;
//     }

//     switch (msg.request)
//     {
//         default:
//             std::cout << "[meatnet::Host] Invalid request: " << msg.request << "\n";
//             break;
    
//         case REQ_DISCONNECT:    _req_disconnect  (idx, msg, buffer);   break;
//         case REQ_FILE:          _req_file        (idx, msg, buffer);   break;
//         case REQ_JOIN:          _req_join        (idx, msg, buffer);   break;
//         case REQ_CREATE_BUF:    _req_create_buf  (idx, msg, buffer);   break;
//         case REQ_BUF_GET:       _req_buf_get     (idx, msg, buffer);   break;
//         case REQ_BUF_SUBDATA:   _req_buf_subdata (idx, msg, buffer);   break;
//     }


//     msg.timestamp = uint32_t(std::time(NULL));

//     if (!msg_send(m_clients[idx], msg, buffer))
//     {
//         std::cout << "rip\n";
//         return;
//     }

//     // std::cout << "[meatnet::Host] Processing " << req_str(msg.request) << " request... success\n";

// }



// void
// meatnet::Host::_main_thread( uint16_t port )
// {
//     TCPsocket server = _open_connection(port);

//     if (!server)
//     {
//         std::cout << "[meatnet::Host] Error opening connection on port " << port << "\n";
//         shutdown();
//         return;
//     }
//     std::cout << "[meatnet::Host] Connection opened on port " << port << "\n";




//     Message msg;
//     void *buffer = std::malloc(BUFFER_SIZE);
//     std::memset(buffer, '\0', BUFFER_SIZE);


//     int idx = 0;
//     uint32_t time_now, time_delta;

//     while (running())
//     {
//         idx = (idx + 1) % MAX_PLAYERS;

//         if (m_clients[idx] == NULL)
//         {
//             TCPsocket client = _client_connect(server);
        
//             if (client)
//             {
//                 m_clients[idx] = client;
//                 m_timers[idx]  = uint32_t(std::time(NULL));
//             }
            
//             continue;
//         }


//         time_now = uint32_t(std::time(NULL));

//         if (msgheader_recv(m_clients[idx], msg, buffer))
//         {
//             m_timers[idx] = time_now;
//             _process_request(idx, msg, buffer);
//         }

//         else if (time_now - m_timers[idx] > TIMEOUT_SECONDS)
//         {
//             SDLNet_TCP_Close(m_clients[idx]);
//             m_clients[idx] = NULL;
//             std::cout << "[meatnet::Host] Disconnected client " << idx << " (timeout)\n";
//         }

//     }


//     for (int i=0; i<MAX_PLAYERS; i++)
//     {
//         if (m_clients[i])
//         {
//             SDLNet_TCP_Close(m_clients[i]);
//         }
//     }


//     SDLNet_TCP_Close(server);
// }


