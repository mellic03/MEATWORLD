#include "game.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_log.hpp>

#include "systems/sys-player.hpp"
#include "menu/menu.hpp"

#include "../../meatnet/src/meatnet.hpp"

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


int root_panel;
idkui2::LayoutManager *LM;
idkui2::Panel *ui_root;

meatnet::Host   *meatnet_host   = nullptr;
meatnet::Client *meatnet_client = nullptr;


int player;
std::vector<int> players = std::vector<int>(meatnet::MAX_PLAYERS, -1);


void
MeatWorldGame::setup( idk::EngineAPI &api )
{
    using namespace idk;

    auto &engine   = api.getEngine();
    auto &eventsys = api.getEventSys();
    auto &ren      = api.getRenderer();

    eventsys.onDropFile(".idksc", [](const char *filepath)
    {
        idk::ECS2::load(filepath);
    });

    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox5/"));
    ren.pushRenderOverlay("assets/meatworld-white.png", 0.5f, 2.0f, 0.5f);

    LM = new idkui2::LayoutManager("./assets/fonts/RodettaStamp.ttf", 32);

    // if (ECS2::getComponentArray<PlayerControllerCmp>().size() == 0)
    {
        LOG_INFO() << "Creating player";
        player = ECS2::createGameObject("Player", false);
        ECS2::giveComponent<PlayerControllerCmp>(player);
    }

    ui_root = createMenu(api, LM, meatnet_host, meatnet_client);




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
    // -----------------------------------------------------------------------------------------


}




void
MeatWorldGame::mainloop( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

    float dt = engine.deltaTime();

    idkui::TextManager::text(10, 10) << "MEATWORLD v0.1.0";

    if (eventsys.keylog().keyTapped(idk::Keycode::SPACE))
    {
        ren.skipRenderOverlay();
    }


    if (meatnet_host)
    {
        idkui::TextManager::text(10, 30) << "Connected as host";
        meatnet_host->update(api, players);
    }

    if (meatnet_client)
    {
        idkui::TextManager::text(10, 30) << "Connected as client";
        meatnet_client->update(api, player, players);
    }

    {
        LM->update(api, dt);

        if (eventsys.mouseCaptured())
        {
            ui_root->close();
        }

        else
        {
            ui_root->open();
        }

        LM->renderTexture(api);
    }

    // ren.drawTextureOverlay(LM->m_atlas);
}



void
MeatWorldGame::shutdown()
{
    LOG_INFO() << "MeatWorldGame::shutdown";

    if (meatnet_host)
    {
        meatnet_host->shutdown();
    }

    if (meatnet_client)
    {
        meatnet_client->shutdown();
    }

    // idk::ui::shutdown();
}

