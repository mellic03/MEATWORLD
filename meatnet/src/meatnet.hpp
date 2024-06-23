#pragma once

#include <SDL2/SDL_net.h>

#include <libidk/idk_glm.hpp>
#include <libidk/memory/common.hpp>
#include <libidk/idk_assert.hpp>

#include <IDKGameEngine/IDKGameEngine.hpp>

#include <vector>
#include <map>

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include <functional>



namespace meatnet
{
    class Base;
    class Host;
    class Client;

    static constexpr size_t MAX_PLAYERS = 4;
    static constexpr size_t BUFFER_SIZE = 16 * idk::KILO * sizeof(uint8_t);
    static constexpr uint32_t TIMEOUT_SECONDS = 8;


    enum Request: uint32_t
    {
        REQ_NONE        = 0 << 0,
        REQ_CONNECT     = 1 << 0,
        REQ_DISCONNECT  = 1 << 1,
        REQ_FILE        = 1 << 2,
        REQ_PLAYERDATA  = 1 << 3,
    };

    enum Response: uint32_t
    {
        RES_NONE        = 0 << 0,
        RES_OKAY        = 1 << 0,
        RES_STRING      = 1 << 1,
        RES_FILE        = 1 << 2,
        RES_PEERDATA    = 1 << 3,
    };

    std::string req_str( uint32_t req );
    std::string res_str( uint32_t res );


    struct Header
    {
        uint32_t timestamp;
        uint32_t request;
        uint32_t response;
        size_t   nbytes;
    };


    struct File
    {
        char   name[64];
        size_t nbytes;
    
        File()
        {
            std::memset(name, '\0', sizeof(name));
        }
    };

    struct PlayerData
    {
        glm::vec3 position;
        float     yaw;
        uint32_t  action;
    };

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


    void     _set_status( uint32_t s ) { m_status.store(s);      };
    uint32_t _get_status(            ) { return m_status.load(); };

    template <typename T>
    bool generic_send( TCPsocket, const T& );

    template <typename T>
    bool generic_recv( TCPsocket, T* );

    void string_recv( TCPsocket, std::string* );
    void string_send( TCPsocket, const std::string& );

    void file_recv( TCPsocket, meatnet::File& );
    void file_send( TCPsocket, const std::string& );

    bool header_send( TCPsocket, const meatnet::Header&, const void* );
    bool header_recv( TCPsocket, meatnet::Header&, void* );

    bool data_send( TCPsocket, const meatnet::Header&, const void* );
    bool data_recv( TCPsocket, meatnet::Header&, void* );

    bool msg_send( TCPsocket, const meatnet::Header&, const void* );
    bool msg_recv( TCPsocket, meatnet::Header&, void* );


public:

    Base()
    {
        m_running.store(true);
    };


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
    std::string             m_filepath;

    std::vector<PlayerData> m_playerdata;
    std::vector<TCPsocket>  m_clients;
    std::vector<uint32_t>   m_timers;

    void      _process_req_disconnect ( int, meatnet::Header&, void* );
    void      _process_req_file       ( int, meatnet::Header&, void* );
    void      _process_req_player     ( int, meatnet::Header&, void* );
    void      _process_request        ( int, meatnet::Header&, void* );



    TCPsocket _open_connection ( uint16_t port );
    TCPsocket _client_connect  ( TCPsocket server );

    void      _msg_thread  ( uint16_t port );
    void      _main_thread ( uint16_t port );

public:
    Host( uint16_t main_port, uint16_t msg_port );

    void loadScene( const std::string& );

};



class meatnet::Client: public meatnet::Base
{
private:
    using callback_type = std::function<void(std::string)>;

    callback_type  m_callback;
    std::string    m_username = "";
    std::string    m_hostname = "";
    std::string    m_filepath = "";

    PlayerData              m_playerdata;
    std::vector<PlayerData> m_peers;


    bool      _make_request( TCPsocket, uint32_t, Header&, void* );

    TCPsocket _connect     ( const char *host, uint16_t port );
    void      _msg_thread  ( const char *host, uint16_t port );
    void      _main_thread ( const char *host, uint16_t port );

public:

    Client( const std::string&, const std::string&, uint16_t, std::function<void(std::string)> );
    void update( meatnet::PlayerData& );

    std::vector<PlayerData> getPeers();

};





template <typename T>
bool
meatnet::Base::generic_send( TCPsocket socket, const T &data )
{
    while (running())
    {
        int sent = SDLNet_TCP_Send(socket, (const void *)(&data), sizeof(T));

        if (sent == sizeof(T))
        {
            return true;
        }

        else if (sent == -1)
        {
            return false;
        }
    }

    return false;
}


template <typename T>
bool
meatnet::Base::generic_recv( TCPsocket socket, T *data )
{
    while (running())
    {
        int received = SDLNet_TCP_Recv(socket, (void *)(data), sizeof(T));

        if (received == sizeof(T))
        {
            return true;
        }

        else if (received == -1)
        {
            return false;
        }
    }

    return false;
}
