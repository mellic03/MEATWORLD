#pragma once

#include <IDKengine/IDKengine.hpp>


class Transform_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<std::tuple<int, int, idk::Transform>>  m_transforms;
    std::map<int, int>  m_keys;

public:
    void        init    ( idk::Engine & ) {  };
    void        stage_A ( idk::Engine & ) {  };
    void        stage_B ( idk::Engine & ) {  };
    void        stage_C ( idk::Engine & ) {  };

    void        onObjectAssignment( int obj_id, idk::Engine & );
    void        onObjectCreation( int obj_id, idk::Engine & );
    void        onObjectDeletion( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    idk::Transform &    getTransform( int obj_id );

    void                translate ( int obj_id, glm::vec3 v );
    void                rotateX   ( int obj_id, float r     );
    void                rotateY   ( int obj_id, float r     );
    void                rotateZ   ( int obj_id, float r     );
    void                scale     ( int obj_id, glm::vec3 s );


    void                setParent   ( int parent_id, int child_id );
    void                unsetParent ( int child_id                );

    glm::mat4           getModelMatrix( int obj_id );

};

