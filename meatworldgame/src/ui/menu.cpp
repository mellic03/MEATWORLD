#include "ui.hpp"

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>

void
createMenu( idk::EngineAPI &api, idkui2::LayoutManager *LM,
            meatnet::Host *&meatnet_host, meatnet::Client *&meatnet_client,
            meatworld::GameData *gamedata, meatworld::GameUI *gameui )
{
    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();


    idkui2::ElementStyle rootstyle = {
        .invisible = true,
        .radius    = 0.0f
    };

    idkui2::ElementStyle style1 = {
        .margin = glm::vec4(8.0f),
        .radius = 16.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };

    idkui2::ElementStyle settingsstyle = {
        .invisible = true,
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 16.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };

    idkui2::ElementStyle liststyle = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 64.0f),
        .radius  = 16.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.1f, 0.1f, 0.1f, 0.95f)
    };

    idkui2::ElementStyle buttonstyle = {
        .margin = glm::vec4(4.0f),
        .radius = 16.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };


    gameui->root = LM->createRootPanel(1, 3, rootstyle);

    gameui->mainmenu    = new idkui2::Grid("Main Menu", settingsstyle, 1, 1);
    auto *mainmenu_list = new idkui2::List("Main Menu List", liststyle);
    gameui->mainmenu->open();

    gameui->multiplayer    = new idkui2::Grid("Multiplayer", settingsstyle, 1, 1);
    auto *multiplayer_list = new idkui2::List("Multiplayer List", liststyle);

    gameui->root->setChild(0, 0, gameui->mainmenu);

    // Main
    // -----------------------------------------------------------------------------------------
    gameui->mainmenu->setChild(0, 0, mainmenu_list);
    mainmenu_list->pushChildFront(new idkui2::Title("Paused", buttonstyle));

    mainmenu_list->pushChildFront(new idkui2::Button("Continue", buttonstyle,
        [&events]()
        {
            events.mouseCapture(true);
        }
    ));

    mainmenu_list->pushChildFront(new idkui2::Button("Multiplayer", buttonstyle,
        [gameui]()
        {
            gameui->root->m_children[0][1] = gameui->multiplayer;
            gameui->multiplayer->open();
        }
    ));

    mainmenu_list->pushChildFront(new idkui2::Button("Settings", buttonstyle,
        [gameui]()
        {
            gameui->root->m_children[0][1] = gameui->settings;
            gameui->settings->open();
        }
    ));

    mainmenu_list->pushChildFront(new idkui2::Button("Exit", buttonstyle,
        [&engine]()
        {
            engine.shutdown();
        }
    ));
    // -----------------------------------------------------------------------------------------




    // Multiplayer
    // -----------------------------------------------------------------------------------------
    gameui->multiplayer->setChild(0, 0, multiplayer_list);
    multiplayer_list->pushChildFront(new idkui2::Title("Multiplayer", buttonstyle));

    // multiplayer_list->pushChildFront(new idkui2::Button("Host", buttonstyle,
    //     [gameui, &meatnet_host]()
    //     {
    //         meatnet_host = new meatnet::Host;
    //         meatnet_host->connect(4200, [](){});
    //     }
    // ));

    multiplayer_list->pushChildFront(new idkui2::Button("Connect", buttonstyle,
        [gamedata, &meatnet_client]()
        {
            auto callback = [gamedata]( std::string filepath )
            {
                gamedata->init_multiplayer(filepath);
            };

            meatnet_client = new meatnet::Client;
            meatnet_client->connect(
                "Michael",
                "127.0.0.1",
                4200,
                callback
            );
        }
    ));

    multiplayer_list->pushChildBack(new idkui2::Button("Return", buttonstyle,
        [gameui]()
        {
            gameui->multiplayer->close();
        }
    ));
    // -----------------------------------------------------------------------------------------



}



