#pragma once

#include <libidk/idk_glm.hpp>
#include <IDKECS/IDKECS.hpp>

#include <SDL2/SDL_net.h>

#include <vector>
#include <map>

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>



namespace meatnet
{
    class Base;
    class Host;
    class Client;

    static constexpr size_t MAX_PLAYERS = 4;

    enum ClientAction: uint32_t
    {
        ACTION_NONE       = 0,
        ACTION_SHOOT      = 1 << 1,
        ACTION_DISCONNECT = 1 << 2
    };

    struct ClientBuffer
    {
        char    header[16];
        char    body[32];
        int32_t response;

        int32_t   player_id;
        glm::vec3 position;
        float     yaw;
        uint32_t  action;
    };

    struct PlayerBuffer
    {
        int32_t   player_id;
        glm::vec3 position;
        float     yaw;
        uint32_t  action;
    };

    struct HandshakeBuffer
    {
        char    header[16];
        char    body[32];
        int32_t response;
    };


    struct File
    {
        std::string name = "";

        size_t  nbytes;
        void   *data;
    };


    HandshakeBuffer HandshakeBuffer_new();
    void HandshakeBuffer_recv( HandshakeBuffer* );
    void HandshakeBuffer_send( HandshakeBuffer* );

    ClientBuffer ClientBuffer_new( int player_id );
    void ClientBuffer_recv( ClientBuffer* );
    void ClientBuffer_send( ClientBuffer* );


};



class meatnet::Base
{
protected:
    std::mutex       m_mutex;
    std::atomic_bool m_running;

    static constexpr uint16_t PORT_MAIN      = 4200;
    static constexpr uint16_t PORT_HANDSHAKE = 4201;

    enum Status: uint32_t
    {
        STATUS_NONE       = 1 << 0,
        STATUS_CONNECTING = 1 << 1,
        STATUS_CONNECTED  = 1 << 2,
        STATUS_GAMEPLAY   = 1 << 3,
        STATUS_ERROR      = 1 << 4,
        STATUS_TIMEOUT    = 1 << 5
    };

    std::atomic_uint32_t             m_status;
    std::vector<ClientBuffer>        m_data;
    std::function<void(std::string)> m_callback = [](std::string) {  };

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



public:

    Base(): m_data(MAX_PLAYERS)
    {
        m_running.store(true);
    };


    uint32_t getStatus() const { return m_status.load(); };


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

    std::vector<int32_t>       m_available_player_ids;
    std::vector<ClientBuffer>  m_players;
    std::map<uint32_t, int>    m_host_map;
    std::string                m_filepath;


    int  _add_player( uint32_t host, std::string username );
    TCPsocket _client_join( TCPsocket server, int idx );

    void _handshake_thread( uint16_t port );
    void _process_request( ClientBuffer *buffer );
    void _server_thread( uint16_t port );

    void _msg_thread  ( uint16_t port );
    void _main_thread ( uint16_t port );

    void _send_players( TCPsocket socket, void *data );

public:
    void update( idk::EngineAPI&, std::vector<int> &players );
    void connect( const std::string&, uint16_t );

};



class meatnet::Client: public meatnet::Base
{
private:
    std::atomic_int32_t        m_player_id;
    std::vector<PlayerBuffer>  m_players;

    ClientBuffer m_buffer;
    std::string  m_name = "";
    std::string  m_filepath = "";

    int  _handshake( const char *host );
    void _client_thread(const char *host, uint16_t port );
    void _write_data( const glm::vec3&, float, uint32_t );
    void _recv_players( TCPsocket socket );

public:
    void update( idk::EngineAPI&, int player, std::vector<int> &players );
    void connect( const std::string&, const char *, uint16_t, std::function<void(std::string)> );

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
