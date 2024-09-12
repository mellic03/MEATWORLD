#pragma once

#include "../character/character.hpp"

namespace meatworld
{
    class PlayerController;
};


class meatworld::PlayerController: public meatworld::CharacterController
{
public:
    virtual CharacterMotion getMovement( idk::EngineAPI& );
};
