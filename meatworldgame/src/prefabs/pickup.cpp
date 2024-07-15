#include "prefabs.hpp"

#include "../game.hpp"


int
meatworld::prefab_pickup()
{
    using namespace idk;
    int obj_id  = ECS2::createGameObject("Pickup");
    int trigger = ECS2::createGameObject("trigger");
    
    ECS2::giveComponent<ModelCmp>(obj_id);
    ModelSys::assignModel(obj_id, "assets/models/glock.idkvi");


    ECS2::giveComponent<TriggerCmp>(trigger);

    {
        auto &cmp = ECS2::getComponent<TriggerCmp>(trigger);
        cmp.trigger  = "pickup-trigger";
        cmp.response = "pickup-response";
    }

    ECS2::giveChild(obj_id, trigger);

    {
        TransformSys::getLocalPosition(trigger) = glm::vec3(0.0f, 0.07f, 0.0f);
        TransformSys::getUniformScale(trigger)  = 0.1f;
    }

    return obj_id;
}


bool
meatworld::trigger_pickup( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    if (MeatWorldGame::player == nullptr)
    {
        return false;
    }

    int player = MeatWorldGame::player->m_obj_id;
    int camera = MeatWorldGame::player->m_cam_obj;

    glm::vec3 cam_pos = TransformSys::getPositionWorldspace(camera);
    glm::vec3 cam_dir = TransformSys::getFront(camera);

    glm::vec3 sphere_pos = TransformSys::getPositionWorldspace(obj_id);
    float     sphere_rad = TransformSys::getUniformScale(obj_id);

    if (idk::geometry::raySphereIntersects(cam_pos, cam_dir, sphere_pos, sphere_rad))
    {
        return true;
    }

    return false;
}


void
meatworld::response_pickup( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();

    int w = ren.width();
    int h = ren.height();

    idkui::TextManager::text(w/2, h/2) << "[E] pick up";

    if (api.getEventSys().keylog().keyTapped(idk::Keycode::E))
    {
        auto *player = MeatWorldGame::player;
        player->giveWeapon<Glock>();

        ECS2::deleteGameObject(ECS2::getParent(obj_id));

        // AudioSys::playSound(obj_id, false);
    }
}


