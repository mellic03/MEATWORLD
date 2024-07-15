#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>
#include <IDKBuiltinCS/sys-trigger.hpp>

#include "../systems/sys-grabbable.hpp"
#include "../game.hpp"



namespace meatworld
{
    int  prefab_door();
    bool trigger_door  ( idk::EngineAPI&, int );
    void response_door ( idk::EngineAPI&, int );

    int  prefab_pickup();
    bool trigger_pickup  ( idk::EngineAPI&, int );
    void response_pickup ( idk::EngineAPI&, int );


    template <typename T>
    int prefab_weapon();

    template <typename T>
    bool trigger_weapon( idk::EngineAPI&, int );

    template <typename T>
    void response_weapon( idk::EngineAPI&, int );

    template <typename T>
    void registerWeaponPickup();


    void registerPrefabs();
}




template <typename weapon_type>
void
meatworld::registerWeaponPickup()
{
    using namespace idk;

    GrabbableSys::registerCallback(
        weapon_type::name() + "-pickup-callback",
        meatworld::response_weapon<weapon_type>
    );

    ECS2::registerPrefab(weapon_type::name(), prefab_weapon<weapon_type>);

    TriggerSys::registerTrigger(
        weapon_type::name() + "-pickup-trigger",
        meatworld::trigger_weapon<weapon_type>
    );

    TriggerSys::registerResponse(
        weapon_type::name() + "-pickup-callback",
        meatworld::response_weapon<weapon_type>
    );

}


template <typename weapon_type>
int
meatworld::prefab_weapon()
{
    using namespace idk;

    int obj_id = ECS2::createGameObject(weapon_type::name());
    ECS2::giveComponent<ModelCmp>(obj_id);
    ModelSys::assignModel(obj_id, weapon_type::model_path());

    int pickup_obj = ECS2::createGameObject("pickup");
    ECS2::giveChild(obj_id, pickup_obj);
    ECS2::giveComponent<PickupCmp>(pickup_obj);

    {
        auto &cmp = ECS2::getComponent<PickupCmp>(pickup_obj);

        cmp.name     = weapon_type::name();
        cmp.max_quantity  = 2;
        cmp.curr_quantity = 2;
        cmp.callback = weapon_type::name() + "-pickup-callback";
    }


    {
        TransformSys::getLocalPosition(pickup_obj) = glm::vec3(0.0f, -0.05f, 0.0f);
        TransformSys::getUniformScale(pickup_obj)  = 0.1f;
    }

    return obj_id;
}



template <typename weapon_type>
bool
meatworld::trigger_weapon( idk::EngineAPI &api, int obj_id )
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
    // api.getRenderer().drawSphere(sphere_pos, sphere_rad);

    if (glm::distance(cam_pos, sphere_pos) < 1.0f)
    {
        if (idk::geometry::raySphereIntersects(cam_pos, cam_dir, sphere_pos, sphere_rad))
        {
            auto &ren = api.getRenderer();
            int w = ren.width();
            int h = ren.height();
            idkui::TextManager::text(w/2, h/2) << "[E] pick up " << weapon_type::name();

            if (api.getEventSys().keylog().keyTapped(idk::Keycode::E))
            {
                return true;
            }
        }
    }

    return false;
}


template <typename weapon_type>
void
meatworld::response_weapon( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto *player = MeatWorldGame::player;
    player->giveWeapon<weapon_type>();
    ECS2::getComponent<ModelCmp>(ECS2::getParent(obj_id)).visible = false;
}

