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

// #include "meatnet/meatnet.hpp"
// #include "meatnet/meatnet.hpp"

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
// Meatnet *meatnet = nullptr;
// MeatnetBuffer meatnet_buffers[4];

// meatnet::Host   *meatnet_host   = nullptr;
// meatnet::Client *meatnet_client = nullptr;


int player;
// std::vector<int> players = std::vector<int>(meatnet::MAX_PLAYERS, -1);


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


    if (ECS2::getComponentArray<PlayerControllerCmp>().size() == 0)
    {
        LOG_INFO() << "Creating player";
        player = ECS2::createGameObject("Player", false);
        ECS2::giveComponent<PlayerControllerCmp>(player);
    }

    else
    {
        player = (*(ECS2::getComponentArray<PlayerControllerCmp>().begin())).obj_id;
    }

    // for (int i=0; i<players.size(); i++)
    // {
    //     players[i] = idk::ECS2::createGameObject("Player " + std::to_string(i), false);
    //     idk::ECS2::gameObjectPersistent(players[i], false);

    //     idk::ECS2::giveComponent<idk::OLPlayerControllerCmp>(players[i]);
    //     idk::ECS2::giveComponent<idk::TransformCmp>(players[i]);
    //     TransformSys::setPositionLocalspace(players[i], glm::vec3(0.0f, 0.85f, 2.0f*i));
    // }




    idkui2::ElementStyle style1 = {
        .fg = glm::vec4(1.0f),
        .bg = glm::vec4(0.1f, 0.1f, 0.1f, 0.5f),
        .radius = 16.0f
    };

    idkui2::ElementStyle style2 = {
        .fg = glm::vec4(1.0f),
        .bg = glm::vec4(0.1f, 0.1f, 0.1f, 0.7f),
        .radius = 16.0f
    };


    LM->createPanel("Root-Panel", idkui2::ALIGN_LEFT | idkui2::ALIGN_TOP, style1);
    LM->createPanel("Multiplayer-Panel", idkui2::ALIGN_CENTER, style1);
    LM->createPanel("Settings-Panel", idkui2::ALIGN_CENTER, style1);

    // Main
    // -----------------------------------------------------------------------------------------
    LM->createButton("Root-Panel", "Continue", style2, [&eventsys]()
    {
        eventsys.mouseCapture(true);
    });

    LM->createButton("Root-Panel", "Multiplayer", style2, [=]()
    {
        LM->togglePanel("Multiplayer-Panel");
    });

    LM->createButton("Root-Panel", "Settings", style2, [=]()
    {
        LM->openPanel("Settings-Panel");
    });

    LM->createButton("Root-Panel", "Exit", style2, [&engine]()
    {
        engine.shutdown();
    });
    // -----------------------------------------------------------------------------------------


    // Multiplayer
    // -----------------------------------------------------------------------------------------
    LM->createButton("Multiplayer-Panel", "Host", style2, [=]()
    {
        // meatnet_host = new meatnet::Host;
        // meatnet_host->connect(4200, [](){});
    });

    LM->createButton("Multiplayer-Panel", "Connect", style2, [=]()
    {
        // meatnet_client = new meatnet::Client;
        // meatnet_client->connect("Michael" + std::to_string(rand()%1000), "127.0.0.1", 4200, [](){});
    });

    LM->createButton("Multiplayer-Panel", "Return", style2, [=]()
    {
        LM->closePanel("Multiplayer-Panel");
    });
    // -----------------------------------------------------------------------------------------


    // Settings
    // -----------------------------------------------------------------------------------------
    LM->createButton("Settings-Panel", "Return", style2, [=]()
    {
        LM->closePanel("Settings-Panel");
    });
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


    // if (meatnet_host)
    // {
    //     idkui::TextManager::text(10, 30) << "Connected as host";
    //     meatnet_host->update(api, players);
    // }

    // if (meatnet_client)
    // {
    //     idkui::TextManager::text(10, 30) << "Connected as client";
    //     meatnet_client->update(api, player, players);
    // }

    {
        LM->update(api, dt);

        if (eventsys.mouseCaptured())
        {
            LM->closePanel("Root-Panel");
        }

        else
        {
            LM->openPanel("Root-Panel");
        }

        LM->renderTexture(api);
    }

    ren.drawTextureOverlay(LM->m_atlas);
}



void
MeatWorldGame::shutdown()
{
    LOG_INFO() << "MeatWorldGame::shutdown";

    // if (meatnet)
    // {
    //     meatnet->shutdown();
    // }

    // idk::ui::shutdown();
}

