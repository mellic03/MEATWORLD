#pragma once

#include <IDKGameEngine/IDKengine.hpp>


namespace idkg { class PhysicsBody_CS; }

class idkg::PhysicsBody_CS: public idk::ComponentSystem
{
private:
    

public:

    void        init    ( idk::EngineAPI & );
    void        stage_A ( idk::EngineAPI & );
    void        stage_B ( idk::EngineAPI & ) {  };
    void        stage_C ( idk::EngineAPI & ) {  };

    void        onObjectAssignment         ( int obj_id, idk::Engine & );
    void        onObjectCreation ( int obj_id, idk::Engine & );
    void        onObjectDeletion ( int obj_id, idk::Engine & );
    void        onObjectCopy     ( int src_obj_id, int dest_obj_id, idk::Engine & );
};

