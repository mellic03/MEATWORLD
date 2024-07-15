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
    class CharacterBase;
    class NPC;
    class CharacterController;
};


class meatworld::CharacterBase
{
private:
    CharacterController *m_controller;

protected:

public:
    int m_obj_id = -1;

    // CharacterBase( CharacterController *controller )
    // :   m_controller(controller)
    // {
        
    // }

    virtual void update( idk::EngineAPI& ) = 0;

};



class meatworld::NPC: public meatworld::CharacterBase
{
public:
    virtual void update( idk::EngineAPI& ) final;

};


