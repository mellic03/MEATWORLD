#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include "playercontroller.hpp"

namespace idkg { class PlayerController_CS; };

class idkg::PlayerController_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<CharacterController> m_controllers;
    std::map<int, int> m_controller_ids;

public:

    void        init( idk::EngineAPI & );
    void        stage_A( idk::EngineAPI & );
    void        stage_B( idk::EngineAPI & ) {  };
    void        stage_C( idk::EngineAPI & ) {  };

    void        onObjectAssignment( int obj_id, idk::Engine & );
    void        onObjectCreation( int obj_id, idk::Engine & );
    void        onObjectDeletion( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    CharacterController &getController( int obj_id );
};

