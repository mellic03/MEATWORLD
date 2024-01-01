#include "camera_cs.hpp"
#include "idk_transform_CS.h"


void
idkg::Camera_CS::init( idk::Engine &engine )
{
    addDependencies("transform");
}


void
idkg::Camera_CS::stage_A( idk::Engine &engine )
{
    Transform_CS &tCS = engine.getCS<Transform_CS>();
    idk::RenderEngine &ren = idk::RenderEngine::get();

    for (auto &[obj_id, camera_id]: m_camera_ids)
    {
        idk::Transform &transform = tCS.getTransform(obj_id);
        idk::Camera    &camera    = ren.getCamera(camera_id);

        // Move camera position towards object position
        glm::vec3 opos = transform.position();
        glm::vec3 cpos = camera.position();

        float alpha = 0.5f;

        camera.translate(alpha * (opos - cpos));
    }

}



idk::Camera &
idkg::Camera_CS::getCamera( int obj_id )
{
    int id = m_camera_ids[obj_id];
    return idk::RenderEngine::get().getCamera(id);
}



int
idkg::Camera_CS::getCameraID( int obj_id )
{
    return m_camera_ids[obj_id];
}



void
idkg::Camera_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_camera_ids[obj_id] = idk::RenderEngine::get().createCamera();
}


void
idkg::Camera_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
idkg::Camera_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    std::cout << "[Camera_CS] deleting obj_id: " << obj_id << "\n";
    m_camera_ids[obj_id] = -1;
}


void
idkg::Camera_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}

