#pragma once

#include <IDKengine/IDKengine.hpp>


namespace idkg { class Camera_CS; };

class idkg::Camera_CS: public idk::ComponentSystem
{
private:
    idk::Engine *engineptr;
    std::map<int, int> m_camera_ids;

public:

    void        init( idk::Engine & );
    void        stage_A( idk::Engine & );
    void        stage_B( idk::Engine & )    {  };
    void        stage_C( idk::Engine & )    {  };

    void        onObjectAssignment( int obj_id, idk::Engine & );
    void        onObjectCreation( int obj_id, idk::Engine & );
    void        onObjectDeletion( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );
    
    idk::Camera &getCamera( int obj_id );
    int          getCameraID( int obj_id );
};

