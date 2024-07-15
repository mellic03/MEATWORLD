#pragma once

#include "../character/character.hpp"
#include "flashlight.hpp"
#include "weapon.hpp"
#include <meatnet.hpp>


namespace meatworld
{
    class Player;
    class EditorPlayer;
};



class meatworld::Player: public meatworld::CharacterBase
{
public:
    int m_grabbed_obj = -1;
    int m_hinge_obj   = -1;
    int m_hit_obj     = -1;

    float m_health = 10.0f;

    float m_run_speed  = 3.5f;
    float m_walk_speed = 2.5f;
    float m_jump_force = 1.0f;

    Flashlight m_flashlight;
    // Glock *m_glock = nullptr;
    RangedWeapon *m_weapon = nullptr;


    int m_cam_obj = -1;

    Player();
    ~Player();

    virtual void update( idk::EngineAPI& );
    void update( idk::EngineAPI&, meatnet::PeerData& );

    template <typename T>
    void giveWeapon()
    {
        if (m_weapon)
        {
            delete m_weapon;
        }

        m_weapon = new T(m_cam_obj, m_hit_obj);
    }
};




class meatworld::EditorPlayer: public meatworld::Player
{
public:

    virtual void update( idk::EngineAPI& );
};

