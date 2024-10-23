#pragma once

#include "../character/weapon.hpp"


namespace meat
{
    class Physgun;
}


class meat::Physgun: public meat::WeaponRanged
{
public:
    Physgun( meat::World&, ActorBase* );
    virtual glm::vec2 attack( idk::EngineAPI&, meat::World& ) override;

};
