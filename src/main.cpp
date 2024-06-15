#include "game.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_log.hpp>

#include "systems/sys-player.hpp"
#include <iostream>


void
MeatWorldGame::registerModules( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();

    idk::ECS2::registerSystem<idk::PlayerSys>();
    idk::ECS2::registerComponent<idk::PlayerControllerCmp>("PlayerController");
    idk::ECS2::registerComponent<idk::PlayerArmsCmp>("PlayerArms");
    idk::ECS2::registerComponent<idk::ArmCmp>("Arm");

    idkg::registeruserCallbacks();
}


void
MeatWorldGame::init( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    
    auto &eventsys = api.getEventSys();
    auto &ren      = api.getRenderer();

    eventsys.onDropFile(".idksc", [](const char *filepath)
    {
        idk::ECS2::load(filepath);
    });

    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox5/"));
    ren.pushRenderOverlay("assets/meatworld.png", 0.5f, 2.0f, 0.5f);
}


void
MeatWorldGame::mainloop( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

    idkui::TextManager::text(10, 10) << "MEATWORLD v0.1.0";

    if (eventsys.keylog().keyTapped(idk::Keycode::SPACE))
    {
        ren.skipRenderOverlay();
    }

    // std::cout << "WOOP: " << idk::TransformSys::getPositionWorldspace(0).x << "\n";

}



void
MeatWorldGame::shutdown()
{
    LOG_INFO() << "MeatWorldGame::shutdown";
}

