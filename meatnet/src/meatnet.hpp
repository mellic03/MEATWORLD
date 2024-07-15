#pragma once

#include <SDL2/SDL_net.h>
#include <glm/glm.hpp>

// #include <libidk/idk_glm.hpp>
// #include <libidk/memory/common.hpp>
// #include <libidk/idk_assert.hpp>

// #include <IDKGameEngine/IDKGameEngine.hpp>

#include <queue>
#include <map>

#include <atomic>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>



namespace meatnet
{
    class Base;
    class Host;
    class Client;


    enum PeerAction: uint32_t
    {
        ACTION_NONE       = 0 << 0,
        ACTION_SHOOT      = 1 << 0,
    };

    enum PeerState: uint32_t
    {
        STATE_NONE       = 0 << 0,
        STATE_CROUCH     = 1 << 0,
        STATE_FLASHLIGHT = 1 << 1
    };

    enum PeerWeapon: uint32_t
    {
        WEAPON_NONE  = 0 << 0,
        WEAPON_GLOCK = 1 << 0,
        WEAPON_AR2   = 1 << 1
    };

    struct PeerData
    {
        uint64_t  timestamp  = 0;
        glm::vec3 position;
        uint8_t   pitch, yaw;

        uint8_t   id     = 0;
        uint8_t   action = 0;
        uint8_t   state  = 0;
        uint8_t   weapon = 0;
    };


    struct ClientRep
    {
        IPaddress address;
        PeerData  data;
    };


    // static constexpr size_t MAX_PLAYERS = 4;
    // static constexpr size_t BUFFER_SIZE = 16 * idk::KILO * sizeof(uint8_t);
    // static constexpr uint32_t TIMEOUT_SECONDS = 8;


    // enum Request: uint32_t
    // {
    //     REQ_NONE        = 0 << 0,
    //     REQ_CONNECT     = 1 << 0,
    //     REQ_DISCONNECT  = 1 << 1,
    //     REQ_FILE        = 1 << 2,
    //     REQ_JOIN        = 1 << 3,
    //     REQ_CREATE_BUF  = 1 << 4,
    //     REQ_BUF_GET     = 1 << 5,
    //     REQ_BUF_SUBDATA = 1 << 6,
    // };

    // enum Response: uint32_t
    // {
    //     RES_NONE        = 0 << 0,
    //     RES_OKAY        = 1 << 0,
    //     RES_REFUSE      = 1 << 1,
    //     RES_STRING      = 1 << 2,
    //     RES_FILE        = 1 << 3,
    //     RES_PEERDATA    = 1 << 4,
    // };

    // std::string req_str( uint32_t req );
    // std::string res_str( uint32_t res );


    // struct Message
    // {
    //     uint32_t timestamp;
    //     uint32_t request;
    //     uint32_t response;
    //     char     header[32];
    //     size_t   nbytes;


    //     Message() {  };


    //     Message( uint32_t req, size_t n )
    //     :   timestamp (uint32_t(std::time(NULL))),
    //         request   (req),
    //         nbytes    (n)
    //     {

    //     }


    //     Message( uint32_t req, size_t n, const std::string &str )
    //     :   timestamp (uint32_t(std::time(NULL))),
    //         request   (req),
    //         nbytes    (n)
    //     {
    //         bool success = (sizeof(str[0])*str.size() <= sizeof(header));
    //         IDK_ASSERT("Reason longer than allowed", success);
    //         std::strcpy(header, str.data());
    //     }

    // };


    // struct SyncBuffer
    // {
    //     size_t stride;
    //     size_t nbytes;
    //     void  *data;
    // };
    

    // struct SyncBufferRequest
    // {
    //     char     name[32];
    //     size_t   stride;
    //     size_t   nbytes;

    //     SyncBufferRequest()
    //     {

    //     };

    //     SyncBufferRequest( const std::string &buffer_name )
    //     {
    //         IDK_ASSERT("buffer_name too large", buffer_name.length() <= 32);
    //         std::strcpy(&(name[0]), buffer_name.c_str());
    //     };

    //     SyncBufferRequest( const std::string &buffer_name, size_t n, size_t s )
    //     {
    //         stride = s;
    //         nbytes = n;

    //         IDK_ASSERT("buffer_name too large", buffer_name.length() <= 32);
    //         std::strcpy(&(name[0]), buffer_name.c_str());
    //     };
    // };


    // struct File
    // {
    //     char   name[64];
    //     size_t nbytes;
    
    //     File()
    //     {
    //         std::memset(name, '\0', sizeof(name));
    //     };
    // };


};



class meatnet::Base
{
protected:
    using lock_type = std::lock_guard<std::mutex>;

    enum Status: uint32_t
    {
        STATUS_NONE       = 1 << 0,
        STATUS_CONNECTING = 1 << 1,
        STATUS_CONNECTED  = 1 << 2,
        STATUS_GAMEPLAY   = 1 << 3,
        STATUS_ERROR      = 1 << 4,
        STATUS_TIMEOUT    = 1 << 5
    };

    std::mutex           m_mutex;
    std::atomic_bool     m_running;
    std::atomic_uint32_t m_status;
    // std::map<std::string, SyncBuffer>  m_sync_buffers;

    // void     _set_status( uint32_t s ) { m_status.store(s);      };
    // uint32_t _get_status(            ) { return m_status.load(); };

    // void     _error( TCPsocket );

    // template <typename T>
    // bool generic_send( TCPsocket, const T& );

    // template <typename T>
    // bool generic_recv( TCPsocket, T* );

    // void string_recv( TCPsocket, std::string* );
    // void string_send( TCPsocket, const std::string& );

    // void file_recv( TCPsocket, meatnet::File& );
    // void file_send( TCPsocket, const std::string& );

    // bool msgheader_send( TCPsocket, const meatnet::Message&, const void* );
    // bool msgheader_recv( TCPsocket, meatnet::Message&, void* );

    // bool data_send( TCPsocket, const meatnet::Message&, const void* );
    // bool data_recv( TCPsocket, meatnet::Message&, void* );

    // bool msg_send( TCPsocket, const meatnet::Message&, const void* );
    // bool msg_recv( TCPsocket, meatnet::Message&, void* );


public:

    Base()
    {
        m_running.store(true);
    };

 
    uint64_t getTime() const
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    uint32_t getStatus() const
    {
        return m_status.load();
    };


    bool running() const
    {
        return m_running.load();
    };


    void shutdown()
    {
        while (m_running.load() == true)
        {
            m_running.store(false);
        }
    };

};



class meatnet::Host: public meatnet::Base
{
private:
    std::string  m_filepath;
    std::map<uint32_t, uint16_t, ClientRep> m_clients;

    void      _in_thread ( uint16_t port );
    void      _out_thread( uint16_t port );

public:
    Host( uint16_t main_port, uint16_t msg_port );
    void loadScene( const std::string& );

};



class meatnet::Client: public meatnet::Base
{
private:
    using callback_type = std::function<void(std::string)>;

    std::mutex     m_send_mutex;
    std::mutex     m_recv_mutex;


    callback_type  m_callback;
    std::string    m_username = "";
    std::string    m_hostname = "";
    std::string    m_filepath = "";
    int            m_clientID = -1;

    int            m_idx = 0;
    PeerData       m_playerdata[8];
    PeerData       m_peerdata;
    PeerData       m_peers[4];

    void      _in_thread ( const char *host, uint16_t port );
    void      _out_thread( const char *host, uint16_t port );


public:

    Client( const std::string&, const std::string&, uint16_t, std::function<void(std::string)> );

    void send( const PeerData& );
    bool recv( PeerData* );

    // /** callback is executed after response is received from server
    //  * 
    // */
    // template <typename T>
    // void createSyncBuffer( const std::string&, size_t, std::function<void()> );

    // template <typename T>
    // bool writeSyncBuffer( const std::string &name, T data );

    // bool readSyncBuffer( const std::string &name, void *buffer );

};



// template <typename T>
// void
// meatnet::Client::createSyncBuffer( const std::string &name, size_t n, std::function<void()> callback )
// {
//     SyncBufferRequest request(name, n, sizeof(T));

//     m_sync_buffers[name] = {
//         .stride = sizeof(T),
//         .nbytes = 0,
//         .data   = nullptr
//     };

//     {
//         lock_type lock(m_mutex);
//         m_user_requests.push(request);
//         m_user_callbacks.push(callback);
//     }

// }


// template <typename T>
// bool
// meatnet::Client::writeSyncBuffer( const std::string &name, T data )
// {
//     lock_type lock(m_mutex);

//     if (m_sync_buffers.contains(name) == false)
//     {
//         return false;
//     }

//     if (m_sync_buffers[name].data == nullptr)
//     {
//         return false;
//     }

//     uint8_t *dst = (uint8_t *)(m_sync_buffers[name].data);
//              dst = dst + m_clientID * sizeof(T);

//     std::memcpy(dst, (void *)(&data), sizeof(T));

//     return true;
// }



// template <typename T>
// bool
// meatnet::Base::generic_send( TCPsocket socket, const T &data )
// {
//     while (running())
//     {
//         int sent = SDLNet_TCP_Send(socket, (const void *)(&data), sizeof(T));

//         if (sent == sizeof(T))
//         {
//             return true;
//         }

//         else if (sent == -1)
//         {
//             return false;
//         }
//     }

//     return false;
// }


// template <typename T>
// bool
// meatnet::Base::generic_recv( TCPsocket socket, T *data )
// {
//     while (running())
//     {
//         int received = SDLNet_TCP_Recv(socket, (void *)(data), sizeof(T));

//         if (received == sizeof(T))
//         {
//             return true;
//         }

//         else if (received == -1)
//         {
//             return false;
//         }
//     }

//     return false;
// }
