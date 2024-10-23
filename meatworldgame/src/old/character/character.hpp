#pragma once

#include <IDKECS/IDKECS.hpp>

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>

#include <libidk/idk_log.hpp>

#include "../systems/sys-player.hpp"
#include "weapon.hpp"



namespace meatworld
{
    struct CharacterMotion;

    class CharacterBehaviour;
    class CharacterController;
    class CharacterBase;
};


struct meatworld::CharacterMotion
{
    bool run    = false;
    bool jump   = false;
    bool crouch = false;
    bool aim    = false;
    bool attack = false;

    glm::vec3 delta = glm::vec3(0.0f);
    float     pitch = 0.0f;
    float     roll  = 0.0f;
    float     yaw   = 0.0f;
};



class meatworld::CharacterBehaviour
{
public:
    virtual void ree( idk::EngineAPI&, CharacterBase* ) = 0;
};


class meatworld::CharacterController
{
public:
    virtual CharacterMotion getMovement( idk::EngineAPI&, CharacterBase*) = 0;
};


class meatworld::CharacterBase
{
private:
    std::vector<CharacterBehaviour> m_behaviours;

protected:
    int m_root_obj   = -1;
    int m_cam_obj    = -1;
    RangedWeapon *m_weapon = nullptr;

public:

    CharacterBase( const std::string &name, const glm::vec3 &pos, float yaw=0.0f )
    {
        m_root_obj = idk::ECS2::createGameObject(name, false);
        idk::ECS2::giveComponent<CharacterCmp>(m_root_obj);
        idk::ECS2::giveComponent<idk::TransformCmp>(m_root_obj);

        idk::TransformSys::getLocalPosition(m_root_obj)    = pos;
        idk::TransformSys::getTransformCmp(m_root_obj).yaw = yaw;
    }

    template <typename behaviour_type>
    void giveBehaviour()
    {
        m_behaviours.push_back(new behaviour_type());
    }

    virtual void move( idk::EngineAPI&, const CharacterMotion& ) = 0;
    virtual void update( idk::EngineAPI& ) = 0;

    constexpr int objID() const { return m_root_obj; }

    template <typename T>
    void giveWeapon()
    {
        if (m_weapon)
        {
            delete m_weapon;
        }

        m_weapon = new T(m_cam_obj);
    }
};


