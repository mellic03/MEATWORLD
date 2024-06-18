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




class Meatnet
{
private:
    int m_status = 0;
    std::vector<glm::vec3> m_positions;

    std::string      m_host;

    std::mutex       m_mutex;
    std::atomic_bool m_running;
    std::atomic_bool m_shutdown;

    void _server_thread( uint16_t port );
    void _client_thread( uint16_t port );


public:
    static constexpr size_t NUM_PLAYERS = 4;

    Meatnet();

    void update( idk::EngineAPI&, std::vector<int>& );

    bool is_server() const { return m_status == 1; };
    bool is_client() const { return m_status == 2; };

    void server_init( uint16_t port );
    void client_init( const std::string &host, uint16_t port );

    bool running() const { return m_running.load(); };
    void shutdown();

    void writeData( const std::vector<glm::vec3> &data );
    void readData( std::vector<glm::vec3> &data );

    void writePosition( int idx, const glm::vec3& );
    glm::vec3 readPosition( int idx );

};



