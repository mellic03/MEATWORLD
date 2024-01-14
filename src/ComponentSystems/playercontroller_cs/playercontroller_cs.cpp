#include "playercontroller_cs.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKBuiltinCS/idk_camera_cs.hpp>



static idk::EngineAPI *api_ptr;
static idk::RenderEngine *renptr;


void
idkg::PlayerController_CS::init( idk::EngineAPI &api )
{
    api_ptr = &api;
    renptr = &api.getRenderer();
}


void
idkg::PlayerController_CS::stage_A( idk::EngineAPI &api )
{
    for (auto &controller: m_controllers)
    {
        controller.update(api);
    }
}


void
idkg::PlayerController_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    auto &ren      = api_ptr->getRenderer();
    auto &eventsys = api_ptr->getEventSys();

    m_controller_ids[obj_id] = m_controllers.create(CharacterController(obj_id));


    auto &camCS = engine.getCS<idk::Camera_CS>();
    auto &cam   = camCS.getCamera(obj_id);
    cam.setOffset(glm::vec3(0.4f, 1.3f, 1.0f));

    ren.useCamera(camCS.getCameraID(obj_id));

    eventsys.onMouseWheel(
        [&cam](float f)
        {
            cam.addOffset(glm::vec3(0.0f, 0.0f, -0.7f*f));
        }
    );
}


void
idkg::PlayerController_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
idkg::PlayerController_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    int controller_id = m_controller_ids[obj_id];
    m_controllers.destroy(controller_id);
}


void
idkg::PlayerController_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}


idkg::CharacterController &
idkg::PlayerController_CS::getController( int obj_id )
{
    return m_controllers.get(m_controller_ids[obj_id]);
}


