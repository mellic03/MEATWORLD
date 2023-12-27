#include "playercontroller_cs.hpp"
#include "../idk_model_CS.hpp"


void
idkg::PlayerController_CS::init( idk::Engine &engine )
{

}


void
idkg::PlayerController_CS::stage_A( idk::Engine &engine )
{
    for (auto &controller: m_controllers)
    {
        controller.update(engine);
    }
}


void
idkg::PlayerController_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_controller_ids[obj_id] = m_controllers.create(CharacterController(obj_id));

    idk::RenderEngine &ren = engine.rengine();
    ren.getCamera().setOffset(glm::vec3(0.4f, 1.3f, 1.0f));

    engine.eventManager().onMouseWheel(
        [&ren](float f)
        {
            ren.getCamera().addOffset(glm::vec3(0.0f, 0.0f, -0.7f*f));
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


