#pragma once

#include "IDK_builtin_CS_common.h"


class Transform_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<idk::Transform>      _transforms;
    std::vector<int>                    _transform_IDs;

public:
    void        init( idk::Engine & )       {  };
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & )    {  };
    void        stage_C( idk::Engine & )    {  };

    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id, idk::Engine & );
    void        onGameObjectDeletion( int obj_id, idk::Engine & );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    idk::Transform &    getTransform( int obj_id ) { return _transforms.get(_transform_IDs[obj_id]); };
    void                translate( int obj_id, glm::vec3 v );

};

