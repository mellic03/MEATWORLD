#pragma once

#include "../character/character.hpp"
#include "flashlight.hpp"
#include "weapon.hpp"

#include <meatnet.hpp>


namespace meatworld
{
    class OLPlayer;
};



class meatworld::OLPlayer
{
public:
    int m_obj_id    = -1;
    int m_model_obj = -1;
    float m_health  = 10.0f;

    Flashlight m_flashlight;
    RangedWeapon *m_weapon = nullptr;

    void init();

    OLPlayer();
    ~OLPlayer();

    void update( idk::EngineAPI&, const meatnet::PeerData& );

    template <typename T>
    void giveWeapon()
    {
        if (m_weapon)
        {
            delete m_weapon;
        }

        m_weapon = new T(m_obj_id);
    }
};

