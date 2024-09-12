#include "prefabs.hpp"

#include <IDKBuiltinCS/sys-audio.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <libidk/idk_geometry.hpp>

#include "../systems/sys-building.hpp"
#include "../game.hpp"


#include <IDKGraphics/UI/idk_ui.hpp>



bool
meatworld::trigger_door( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    return false;

    // if (MeatWorldGame::player == nullptr)
    // {
    //     return false;
    // }

    // int player = MeatWorldGame::player->objID();
    // int camera = MeatWorldGame::player->m_cam_obj;

    // glm::vec3 cam_pos = TransformSys::getPositionWorldspace(camera);
    // glm::vec3 cam_dir = TransformSys::getFront(camera);

    // glm::vec3 sphere_pos = TransformSys::getPositionWorldspace(obj_id);
    // float     sphere_rad = TransformSys::getUniformScale(obj_id);

    // api.getRenderer().drawSphere(sphere_pos, sphere_rad);

    // return idk::geometry::raySphereIntersects(cam_pos, cam_dir, sphere_pos, sphere_rad);
}



void
meatworld::response_door( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();

    int w = ren.width();
    int h = ren.height();


    int parent = idk::ECS2::getParent(obj_id);
    auto &cmp = idk::ECS2::getComponent<DoorCmp>(parent);

    if (cmp.is_open)
    {
        idkui::TextManager::text(w/2, h/2) << "[E] close door";
    }

    else
    {
        idkui::TextManager::text(w/2, h/2) << "[E] open door";
    }

    if (api.getEventSys().keylog().keyTapped(idk::Keycode::E))
    {
        AudioSys::playSound(obj_id, false);
        cmp.is_open = !cmp.is_open;
    }
}





int
meatworld::prefab_door()
{
    using namespace idk;

    int obj_id = ECS2::createGameObject("Door");
    ECS2::giveComponent<TransformCmp>(obj_id);
    ECS2::giveComponent<ModelCmp>(obj_id);
    ECS2::giveComponent<DoorCmp>(obj_id);
    ModelSys::assignModel(obj_id, "assets/models/doors/door.idkvi");

    int handle = ECS2::createGameObject("handle-trigger");
    ECS2::giveComponent<TransformCmp>(handle);
    ECS2::giveComponent<TriggerCmp>(handle);
    ECS2::giveComponent<AudioEmitterCmp>(handle);
    AudioSys::assignSound(handle, "assets/audio/door-toggle.wav");

    auto &cmp = ECS2::getComponent<TriggerCmp>(handle);
    cmp.trigger  = "door-trigger";
    cmp.response = "door-response";

    ECS2::giveChild(obj_id, handle);
    TransformSys::getLocalPosition(handle) = glm::vec3(-0.75f, 0.98f, 0.0f);
    TransformSys::getUniformScale(handle)  = 0.15f;


    int collider = ECS2::createGameObject("collider");
    ECS2::giveComponent<TransformCmp>(collider);
    ECS2::giveComponent<StaticRectCmp>(collider);

    ECS2::giveChild(obj_id, collider);
    TransformSys::getLocalPosition(collider) = glm::vec3(-0.43f, 0.98f, 0.0f);
    TransformSys::getXYZScale(collider)      = glm::vec3(0.89f, 2.0f, 0.08f); 

    return obj_id;
}




