#include "net-ecs.hpp"



int
meatnet::netECS::createGameObject( const std::string &name, bool persistent )
{
    int obj_id = idk::ECS2::createGameObject(name, persistent);

    

    return obj_id;
}
