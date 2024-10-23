#include "physgun.hpp"

using namespace meat;



Physgun::Physgun( meat::World &world, ActorBase *owner )
:   WeaponRanged(world, owner)
{

}


glm::vec2
Physgun::attack( idk::EngineAPI &api, meat::World &world )
{
    return glm::vec2(0.0f);
}
