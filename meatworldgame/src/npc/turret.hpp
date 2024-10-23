#pragma once

#include "../behaviour/behaviour.hpp"
#include "../character/base.hpp"
#include "../character/character.hpp"
#include "../character/weapon.hpp"

#include <libidk/idk_signal.hpp>


namespace meat
{
    class CharacterTurret;
}


class meat::CharacterTurret: public ActorBase
{
private:
    WeaponAR2 *m_weapon;

public:
    CharacterTurret( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

    virtual void update( idk::EngineAPI&, meat::World& ) override;
    virtual void lookTo( const glm::vec3& );

};


