#pragma once

#include "IDK_builtin_CS_common.h"


class Grabbable_CS: public idk::ComponentSystem
{
private:


public:

    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );
    void        stage_C( idk::Engine & )    {  };

    void        onObjectAssignment( int obj_id, idk::Engine & ) {  };
    void        onObjectCreation( int obj_id, idk::Engine & ) {  };
    void        onObjectDeletion( int obj_id, idk::Engine & ) {  };
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & ) {  };
};

