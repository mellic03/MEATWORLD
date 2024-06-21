#include "menu.hpp"

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>


idkui2::Panel *createMenu( idk::EngineAPI &api, idkui2::LayoutManager *LM,
                           meatnet::Host *&meatnet_host, meatnet::Client *&meatnet_client )
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


    auto *root = LM->createRootPanel(1, 3, rootstyle);

    auto *mainmenu      = new idkui2::Panel("Main Menu", settingsstyle, 1, 1);
    auto *mainmenu_list = new idkui2::List("Main Menu List", liststyle);

    auto *multiplayer      = new idkui2::Panel("Multiplayer", settingsstyle, 1, 1);
    auto *multiplayer_list = new idkui2::List("Multiplayer List", liststyle);

    auto *settings      = new idkui2::Panel("Settings", settingsstyle, 1, 1);
    auto *settings_list = new idkui2::List("Settings List", liststyle);

    root->giveChild(0, 0, mainmenu);
    root->giveChild(0, 1, settings);
    root->giveChild(0, 2, multiplayer);


    // Main
    // -----------------------------------------------------------------------------------------
    mainmenu->giveChild(0, 0, mainmenu_list);
    mainmenu_list->pushChildFront(new idkui2::Title("Paused", buttonstyle));

    mainmenu_list->pushChildFront(new idkui2::Button("Continue", buttonstyle,
        [&events]()
        {
            events.mouseCapture(true);
        }
    ));

    mainmenu_list->pushChildFront(new idkui2::Button("Multiplayer", buttonstyle,
        [multiplayer]()
        {
            multiplayer->open();
        }
    ));

    mainmenu_list->pushChildFront(new idkui2::Button("Settings", buttonstyle,
        [settings]()
        {
            settings->open();
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
    multiplayer->giveChild(0, 0, multiplayer_list);
    multiplayer_list->pushChildFront(new idkui2::Title("Multiplayer", buttonstyle));

    multiplayer_list->pushChildFront(new idkui2::Button("Host", buttonstyle,
        [multiplayer, &meatnet_host]()
        {
            meatnet_host = new meatnet::Host;
            meatnet_host->connect(4200, [](){});
        }
    ));

    multiplayer_list->pushChildFront(new idkui2::Button("Connect", buttonstyle,
        [multiplayer, &meatnet_client]()
        {
            meatnet_client = new meatnet::Client;
            meatnet_client->connect("Michael", "127.0.0.1", 4200, [](){});
        }
    ));

    multiplayer_list->pushChildBack(new idkui2::Button("Return", buttonstyle,
        [multiplayer]()
        {
            multiplayer->close();
        }
    ));
    // -----------------------------------------------------------------------------------------




    // Settings
    // -----------------------------------------------------------------------------------------
    settings->giveChild(0, 0, settings_list);
    settings_list->pushChildFront(new idkui2::Title("Settings", buttonstyle));


    settings_list->pushChildFront(new idkui2::Button("Audio", buttonstyle,
        [settings]()
        {
            // settings->close();
        }
    ));

    settings_list->pushChildFront(new idkui2::Button("Graphics", buttonstyle,
        [settings]()
        {
            // settings->close();
        }
    ));

    settings_list->pushChildBack(new idkui2::Button("Return", buttonstyle,
        [settings]()
        {
            settings->close();
        }
    ));
    // -----------------------------------------------------------------------------------------



    return root;
}



