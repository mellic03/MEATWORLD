#pragma once

#include "IDK_builtin_CS_common.h"


class Transform_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<idk::Transform>      m_transforms;
    std::vector<int>                    m_transform_IDs;
    std::vector<int>                    m_parent_IDs;

public:
    void        init    ( idk::Engine & ) {  };
    void        stage_A ( idk::Engine & ) {  };
    void        stage_B ( idk::Engine & ) {  };
    void        stage_C ( idk::Engine & ) {  };

    void        onObjectAssignment( int obj_id, idk::Engine & );
    void        onObjectCreation( int obj_id, idk::Engine & );
    void        onObjectDeletion( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    idk::Transform &    getTransform( int obj_id ) { return m_transforms.get(m_transform_IDs[obj_id]); };
    void                translate( int obj_id, glm::vec3 v );


    void                setParent   ( int parent_id, int child_id );
    void                unsetParent ( int child_id                );

    glm::mat4           getModelMatrix( int obj_id );

};

