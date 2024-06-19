#pragma once

#include <IDKECS/IDKECS.hpp>



namespace meatnet
{
    class netECS;
};



class meatnet::netECS
{
private:

    enum Action: uint32_t
    {
        ACTION_NONE   = 0 << 0,
        ACTION_CREATE = 1 << 0,
        ACTION_DELETE = 1 << 1
    };


public:

    int createGameObject( const std::string &name, bool persistent = false );

};
