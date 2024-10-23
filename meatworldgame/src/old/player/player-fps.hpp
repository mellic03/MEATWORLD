#pragma once

#include "../character/character.hpp"
#include "../character/humanoid.hpp"

#include "player-controller.hpp"
#include "flashlight.hpp"


namespace meatworld
{
    class PlayerFPS;
};


class meatworld::PlayerFPS: public meatworld::CharacterBase
{
private:

public:
    PlayerFPS( const glm::vec3 &position, float yaw );
    ~PlayerFPS();

    virtual void move( idk::EngineAPI &api, const CharacterMotion &motion );
    virtual void update( idk::EngineAPI &api );

};
