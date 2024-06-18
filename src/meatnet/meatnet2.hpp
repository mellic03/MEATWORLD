#pragma once


#include "client.hpp"
#include "server.hpp"


#include <libidk/idk_glm.hpp>
#include <IDKECS/IDKECS.hpp>


#include <SDL2/SDL_net.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>



namespace mt
{


};



namespace meatnet2
{
    class Interface;
    class Host;
    class Client;


    struct ClientBuffer
    {
        glm::vec3 position;
    };

    struct HostBuffer
    {
        glm::vec3 position;
    };


    void HostBuffer_recv( HostBuffer* );
    void HostBuffer_send( HostBuffer* );

    void ClientBuffer_recv( ClientBuffer* );
    void ClientBuffer_send( ClientBuffer* );


};



class meatnet2::Interface
{
protected:
    std::mutex       m_mutex;
    std::atomic_bool m_running;
    std::atomic_bool m_shutdown;

    void _server_thread( uint16_t port );
    void _client_thread( uint16_t port );


public:
    virtual void update( idk::EngineAPI& ) = 0;
    virtual void connect( const char *host, uint16_t port ) = 0;

    void shutdown()
    {
        while (m_running.load() == true)
        {
            m_shutdown.store(true);
        }
    }

};


class meatnet2::Host: public meatnet2::Interface
{
private:

public:
    virtual void update( idk::EngineAPI& );
    virtual void connect( const char *host, uint16_t port );


};


class meatnet2::Client: public meatnet2::Interface
{
private:

public:
    virtual void update( idk::EngineAPI& );
    virtual void connect( const char *host, uint16_t port );


};




