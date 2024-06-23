#include "ui.hpp"

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>

void
createMenu( idk::EngineAPI &api, idkui2::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI *gameui = &(gamedata->gameui);

    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();


    gameui->root = new idkui2::Grid("Root", meatui::root_style, 3, 4);
    gameui->root->setRowProportions({0.1, 0.8, 0.1});
    gameui->root->setColProportions({0.25, 0.25, 0.25, 0.25});

    gameui->mainmenu = new idkui2::List("Main Menu", meatui::list_style);
    gameui->root->setChild(1, 0, gameui->mainmenu);


    // Main
    // -----------------------------------------------------------------------------------------
    gameui->mainmenu->pushChildFront(new idkui2::Title("Paused", meatui::title_style));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Continue", meatui::button_style,
        [&events]()
        {
            events.mouseCapture(true);
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Multiplayer", meatui::button_style,
        [gameui]()
        {
            gameui->root->m_children[1][1] = gameui->multiplayer;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Settings", meatui::button_style,
        [gameui]()
        {
            gameui->root->m_children[1][1] = gameui->settings;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Exit", meatui::button_style,
        [&engine]()
        {
            engine.shutdown();
        }
    ));
    // -----------------------------------------------------------------------------------------




    // Multiplayer
    // -----------------------------------------------------------------------------------------
    // gameui->multiplayer->pushChildFront(new idkui2::Title("Multiplayer", meatui::title_style));

    // auto *splitwindow = new idkui2::Split("", meatui::splitwindow_style, 0.35f);
    // multiplayer_list->pushChildFront(splitwindow);


    // splitwindow->setLeft(new idkui2::TextInput(
    //     "hostname", meatui::textinput_style, gamedata->meatnet_hostname
    // ));

    // splitwindow->setRight(new idkui2::Button("Connect", meatui::button_style,
    //     [gamedata]()
    //     {
    //         auto callback = [gamedata]( std::string filepath )
    //         {
    //             gamedata->init_multiplayer(filepath);
    //         };

    //         gamedata->meatnet = new meatnet::Client(
    //             "Michael", gamedata->meatnet_hostname, 4200, callback
    //         );
    //     }
    // ));


    // multiplayer_list->pushChildBack(new idkui2::Button("Return", meatui::button_style,
    //     [gameui]()
    //     {
    //         gameui->root->m_children[1][1] = nullptr;
    //     }
    // ));
    // -----------------------------------------------------------------------------------------



}



