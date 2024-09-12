#pragma once

#include <IDKECS/IDKECS.hpp>

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>

#include <libidk/idk_log.hpp>



namespace meatworld
{
    struct CharacterMotion;

    class CharacterBase;
    class NPC;
    class CharacterController;
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


class meatworld::CharacterController
{
public:
    virtual CharacterMotion getMovement( idk::EngineAPI& ) = 0;
};



class meatworld::CharacterBase
{
private:
    CharacterController *m_controller;

protected:
    int m_root_obj = -1;

public:
    virtual void move( idk::EngineAPI&, const CharacterMotion& ) = 0;

    constexpr int objID() const { return m_root_obj; }

};



class meatworld::NPC: public meatworld::CharacterBase
{
public:
    virtual void move( idk::EngineAPI&, const CharacterMotion& ) final;

};


