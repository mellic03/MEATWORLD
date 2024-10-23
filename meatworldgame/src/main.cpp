#include <iostream>

#include "game.hpp"

#include <IDKIO/IDKIO.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKGraphics/renderstage/ssr.hpp>
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_log.hpp>
#include <libidk/idk_random.hpp>

#include "systems/sys-player.hpp"
#include "systems/sys-grabbable.hpp"
#include "systems/sys-building.hpp"
#include "systems/sys-weapon.hpp"
#include "systems/sys-projectile.hpp"
#include "systems/sys-vehicle.hpp"
#include "systems/sys-actor.hpp"
#include "systems/draw.hpp"

#include "world/world.hpp"
#include "character/character.hpp"
#include "vehicle/vehicle.hpp"
#include "item/health.hpp"

#include "player/player.hpp"
#include "npc/humanoid.hpp"
#include "npc/drone.hpp"
#include "npc/turret.hpp"

#include "physics/rigidbody.hpp"


static idkui::LayoutManager *LM = nullptr;
static meat::World *world = nullptr;


void
MeatWorldGame::registerModules( idk::EngineAPI &api )
{
    using namespace idk;
    using namespace meatworld;
    using namespace meat;

    ECS2::registerPrefab("Truck", []()
    {
        auto *V = world->addObject<meat::VehicleTruck>(glm::vec3(-35, 0, 35));
        return V->getID();
    });

    ECS2::registerSystem<idk::GrabbableSys>();
    ECS2::registerSystem<meatworld::BuildingSys>();
    ECS2::registerSystem<meatworld::WeaponSys>();
    ECS2::registerSystem<meatworld::PlayerSys>();
    ECS2::registerSystem<meat::ProjectileSys>();

    ECS2::registerComponent<idk::PlayerSpawnCmp> ("PlayerSpawn",      "Meatworld");
    ECS2::registerComponent<idk::GrabbableCmp>   ("Grabbable",        "Meatworld");
    ECS2::registerComponent<PickupCmp>           ("Pickup",           "Meatworld");
    ECS2::registerComponent<DoorCmp>             ("Door",             "Meatworld");
    ECS2::registerComponent<WallCmp>             ("Wall",             "Meatworld");
    ECS2::registerComponent<WeaponCmp>           ("Weapon",           "Meatworld");
    ECS2::registerComponent<HitSphereCmp>        ("HitSphere",        "Meatworld");
    ECS2::registerComponent<HitBoxCmp>           ("HitBox",           "Meatworld");
    ECS2::registerComponent<CharacterHitBoxCmp>  ("Character HitBox", "Meatworld");
    ECS2::registerComponent<PlayerCmp>           ("NPC",              "Meatworld");
    ECS2::registerComponent<CharacterCmp>        ("Character",        "Meatworld");
    ECS2::registerComponent<VehicleCmp>          ("Vehicle",          "Meatworld");
    ECS2::registerComponent<ActorCmp>            ("Actor",            "Meatworld");
    ECS2::registerComponent<WeaponRangedCmp>     ("WeaponRanged",     "Meatworld");

    ECS2::getComponentArray<PlayerCmp>().userCallback = drawComponent_PlayerCmp;
    ECS2::getComponentArray<CharacterCmp>().userCallback = drawComponent_CharacterCmp;
    ECS2::getComponentArray<VehicleCmp>().userCallback = drawComponent_VehicleCmp;
    ECS2::getComponentArray<ActorCmp>().userCallback = drawComponent_ActorCmp;
    ECS2::getComponentArray<WeaponRangedCmp>().userCallback = drawComponent_WeaponRangedCmp;

}


void
MeatWorldGame::setup( const std::vector<std::string> &args, idk::EngineAPI &api )
{
    using namespace idk;

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    static bool editor_mode = false;

    for (auto &arg: args)
    {
        if (arg == "lm")
        {
            editor_mode = true;
        }
    }

    // ren.createRenderStage(new idk::RenderStageBlueTint);

    LM = new idkui::LayoutManager("./assets/fonts/RodettaStamp.ttf", 24);
    world = new meat::World(api);

    if (editor_mode)
    {
        world->addObject<meat::CharacterFreecam>(glm::vec3(-14, 14, 32));
    }

    else
    {
        world->addObject<meat::CharacterPlayerFPS>(glm::vec3(-14, 14, 32));
    }

    world->addObject<meat::CharacterDrone>(glm::vec3(0, 16, 28));
    world->addObject<meat::CharacterTurret>(glm::vec3(18, 16, 60));

    world->addObject<meat::VehicleTruck>(glm::vec3(-35, 0, 35));
    world->addObject<meat::VehicleCanoe>(glm::vec3(-4, 0, -4));
    world->addObject<meat::VehicleShipPinnacle>(glm::vec3(-27, 0, -26));

    world->addObject<meat::ItemWrapper>(glm::vec3(8, 0, 0), new meat::ItemHealth);
    world->addPhysicsBody<meat::RigidBody>(glm::vec3(-14, 18, 32), glm::vec3(4.0f, 1.75f, 2.0f));

    // ren.pushRenderOverlay("assets/meatworld-white.png", 0.5f, 2.0f, 0.5f);
    ren.pushRenderOverlayFill(glm::vec3(0.0f), 0.0f, 6.25f, 0.25f);
    ren.pushRenderOverlay("assets/dream-demon-logo.png", 0.5f, 4.0f, 0.5f);
}




void
MeatWorldGame::mainloop( idk::EngineAPI &api )
{
    using namespace idk;

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    float dt     = engine.deltaTime();

    world->update(api);


    if (idkio::keyTapped(idk::Keycode::SPACE))
    {
        // ren.skipRenderOverlay();
    }


    static float accum = 0.0f;
    accum += dt;

    if (accum >= 1.0f / 30.0f)
    {
        glm::vec2 md = idkio::mouseDelta();
        world->ui.crosshair->addOffset(0.5f*md);

        LM->updateInput(world->ui.menu_ui->top());
        LM->clearTexture(api, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // LM->clearTexture(api, GL_DEPTH_BUFFER_BIT);
        LM->renderNode(world->ui.ingame_ui, 2);
        LM->renderNode(world->ui.crosshair, 10);
        // LM->clearTexture(api, GL_DEPTH_BUFFER_BIT);
        LM->renderNode(world->ui.hitmarker_img, 10);
        LM->renderTexture(api);

        LM->clearTexture(api, GL_DEPTH_BUFFER_BIT);
        LM->renderNode(world->ui.onscreen_text);
        LM->renderTexture(api);

        // LM->clearTexture(api, GL_DEPTH_BUFFER_BIT);
        LM->renderNode(world->ui.menu_ui, 15);
        LM->renderTexture(api);
    }

}



void
MeatWorldGame::shutdown()
{
    LOG_INFO() << "MeatWorldGame::shutdown";

}
