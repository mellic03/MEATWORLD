#include "IDKGame.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include "character/character.hpp"
#include <IDKGraphics/UI/idk_ui.hpp>

#include <libidk/idk_log.hpp>

#include <iostream>


void
IDKGame::config()
{

}


void
IDKGame::registerModules( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
}



void
IDKGame::setup( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ecs      = api.getECS();
    auto &eventsys = api.getEventSys();
    auto &ren      = api.getRenderer();

    eventsys.onDropFile(".idksc", [&ecs](const char *filepath)
    {
        ecs.readFile(filepath);
    });

    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox5/"));

    ren.pushRenderOverlay("assets/meatworld.png", 0.5f, 2.0f, 0.5f);
}


void
IDKGame::mainloop( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

    idkui::TextManager::text(10, 10) << "IDKGameEngine demo scene";


    if (eventsys.keylog().keyTapped(idk::Keycode::SPACE))
    {
        ren.skipRenderOverlay();
    }

    std::cout << "WOOP: " << idk::TransformSys::getPositionWorldspace(0).x << "\n";

}



void
IDKGame::shutdown()
{
    LOG_INFO() << "IDKGame::shutdown";
}

