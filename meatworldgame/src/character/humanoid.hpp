#pragma once

#include "character.hpp"
#include "limb.hpp"
#include "proc-arm.hpp"
#include "../player/weapon.hpp"


namespace meatworld
{
    class Humanoid;
}


class meatworld::Humanoid: public meatworld::CharacterBase
{
protected:
    int m_torso_obj = -1;
    int m_head_obj  = -1;
    int m_hit_obj   = -1;

    float m_run_speed  = 3.0f;
    float m_walk_speed = 2.0f;
    float m_jump_force = 1.0f;

    float m_health = 100.0f;

    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);

    ProcArmTest *m_arms[2];
    ProcLegController m_leg_controller;

    RangedWeapon *m_weapon = nullptr;


public:

    Humanoid();
    ~Humanoid();


    void update( idk::EngineAPI& );
    virtual void move( idk::EngineAPI&, const CharacterMotion& );

    void reachFor( const glm::vec3& );

    template <typename T>
    void giveWeapon()
    {
        if (m_weapon)
        {
            delete m_weapon;
        }

        m_weapon = new T(m_arms[1]->objID());
    }
};



