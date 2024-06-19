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
    std::atomic_bool m_shutdown;

    static constexpr uint16_t PORT_MAIN      = 4200;
    static constexpr uint16_t PORT_HANDSHAKE = 4201;

    enum Status: uint32_t
    {
        STATUS_NONE       = 1 << 0,
        STATUS_CONNECTING = 1 << 1,
        STATUS_CONNECTED  = 1 << 2,
        STATUS_ERROR      = 1 << 3,
        STATUS_TIMEOUT    = 1 << 4
    };

    std::atomic_uint32_t      m_status;
    std::vector<ClientBuffer> m_data;
    std::function<void()>     m_callback = []() {  };

    void     _set_status( uint32_t s ) { m_status.store(s);      };
    uint32_t _get_status(            ) { return m_status.load(); };


public:

    Base(): m_data(MAX_PLAYERS)
    {
        m_running.store(true);
        m_shutdown.store(false);
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
            m_shutdown.store(true);
        }
    };

};



class meatnet::Host: public meatnet::Base
{
private:

    std::vector<int32_t>       m_available_player_ids;
    std::vector<ClientBuffer>  m_players;
    std::map<uint32_t, int>    m_host_map;

    int  _add_player( uint32_t host, std::string username );

    void _handshake( UDPsocket );
    void _handshake_thread( uint16_t port );
    void _process_request( ClientBuffer *buffer );
    void _server_thread( uint16_t port );
    void _send_players( TCPsocket socket, void *data );

public:
    void update( idk::EngineAPI&, std::vector<int> &players );
    void connect( uint16_t port, std::function<void()> callback );

};



class meatnet::Client: public meatnet::Base
{
private:
    std::atomic_int32_t        m_player_id;
    std::vector<PlayerBuffer>  m_players;

    ClientBuffer m_buffer;
    std::string  m_name = "";

    int  _handshake( const char *host );
    void _client_thread(const char *host, uint16_t port );
    void _write_data( const glm::vec3&, float, uint32_t );
    void _recv_players( TCPsocket socket );

public:
    void update( idk::EngineAPI&, int player, std::vector<int> &players );
    void connect( const std::string &name, const char *host, uint16_t port, std::function<void()> callback );

};




