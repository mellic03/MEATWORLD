#include "game.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_log.hpp>

#include "systems/sys-player.hpp"
#include "ui/ui.hpp"

#include <iostream>


void
MeatWorldGame::registerModules( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();

    idk::ECS2::registerSystem<idk::PlayerSys>();
    idk::ECS2::registerComponent<idk::PlayerControllerCmp>("PlayerController");
    idk::ECS2::registerComponent<idk::OLPlayerControllerCmp>("OLPlayerController");
    idk::ECS2::registerComponent<idk::PlayerArmsCmp>("PlayerArms");
    idk::ECS2::registerComponent<idk::ArmCmp>("Arm");

    idkg::registeruserCallbacks();
}



idkui2::LayoutManager *LM;
meatworld::GameData gamedata;



void
MeatWorldGame::setup( idk::EngineAPI &api )
{
    using namespace idk;

    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();

    events.onDropFile(".idksc", [](const char *filepath)
    {
        idk::ECS2::load(filepath);
    });


    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox8/"));
    ren.pushRenderOverlay("assets/meatworld-white.png", 0.5f, 2.0f, 0.5f);

    LM = new idkui2::LayoutManager("./assets/fonts/RodettaStamp.ttf", 32);

    gamedata.reset_player();


    createMenu(api, LM, &gamedata);
    createSettings(api, LM, &gamedata);
    createMultiplayer(api, LM, &gamedata);
    createSyndromes(api, LM, &gamedata);




    // Heightmap
    // -----------------------------------------------------------------------------------------
    static const idk::glTextureConfig config = {
        .internalformat = GL_SRGB8_ALPHA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .wrap_s         = GL_CLAMP_TO_BORDER,
        .wrap_t         = GL_CLAMP_TO_BORDER,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = GL_FALSE
    };


    idk::TextureWrapper wrapper;
    idk::gltools::loadTexture("assets/heightmaps/sand-dunes.jpg", config, &wrapper);
    idk::PhysicsSys::bakeHeightmap(wrapper);
    // -----------------------------------------------------------------------------------------


}




void
MeatWorldGame::mainloop( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &events = api.getEventSys();

    float dt = engine.deltaTime();

    idkui::TextManager::text(10, 10) << "MEATWORLD v0.1.0";

    if (events.keylog().keyTapped(idk::Keycode::SPACE))
    {
        ren.skipRenderOverlay();
    }


    if (gamedata.player == -1)
    {
        gamedata.reset_player();
    }

    // std::cout << "[MeatWorldGame::mainloop] Player ID: " << gamedata.player << "\n";

    if (gamedata.meatnet)
    {
        idkui::TextManager::text(10, 30) << "Connected as client";

        meatnet::PlayerData data = {
            .position = idk::TransformSys::getPositionWorldspace(gamedata.player),
            .yaw      = idk::TransformSys::getData(gamedata.player).yaw,
            .action   = 1
        };

        gamedata.meatnet->update(data);

        auto peers = gamedata.meatnet->getPeers();

        for (int i=0; i<peers.size(); i++)
        {
            idk::TransformSys::setPositionWorldspace(gamedata.players[i], peers[i].position);
            idk::TransformSys::getData(gamedata.players[i]).yaw = peers[i].yaw;
        }
    }

    if (events.mouseCaptured())
    {
        gamedata.gameui.root->close();
    }

    else
    {
        gamedata.gameui.root->open();
    }


    LM->renderTexture(api, gamedata.gameui.root);

}



void
MeatWorldGame::shutdown()
{
    LOG_INFO() << "MeatWorldGame::shutdown";

    if (gamedata.meatnet)
    {
        gamedata.meatnet->shutdown();
    }

}
