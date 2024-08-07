#pragma once

#include "../character/character.hpp"
#include "flashlight.hpp"
#include "weapon.hpp"
#include <meatnet.hpp>


namespace meatworld
{
    class PlayerTPS;
};



class meatworld::PlayerTPS: public meatworld::CharacterBase
{
private:
    int m_audio_emitter = -1;

public:
    int m_grabbed_obj = -1;
    int m_hinge_obj   = -1;
    int m_hit_obj     = -1;
    int m_cam_obj     = -1;

    float m_health = 10.0f;

    float m_run_speed  = 3.5f;
    float m_walk_speed = 2.5f;
    float m_jump_force = 1.0f;

    Flashlight m_flashlight;
    RangedWeapon *m_weapon = nullptr;


    PlayerTPS();
    ~PlayerTPS();

    virtual void move( idk::EngineAPI&, const glm::vec3&, float dx, float dy );
    void update( idk::EngineAPI& );
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


