#pragma once

#include "../character/character.hpp"
#include "../character/humanoid.hpp"

#include "player-controller.hpp"
#include "flashlight.hpp"
#include "weapon.hpp"


namespace meatworld
{
    class PlayerFPS;


};


class meatworld::PlayerFPS
{
private:
    int m_cam_obj = -1;

public:
    PlayerFPS();
    ~PlayerFPS();

    virtual void move( idk::EngineAPI &api, const CharacterMotion &motion );
};