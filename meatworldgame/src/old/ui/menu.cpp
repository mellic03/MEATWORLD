#include "ui.hpp"

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>



void
createMenu( idk::EngineAPI &api, idkui::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI *gameui = &(gamedata->gameui);
    
    auto *&root = gameui->ingame_menu;

    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();


    root = new idkui::Grid("Root", meatui::root_style, 3, 4);
    root->setRowProportions({0.1, 0.8, 0.1});
    root->setColProportions({0.25, 0.5, 0.125, 0.125});

    gameui->mainmenu = new idkui::List("Main Menu", meatui::list_style);
    root->setChild(1, 0, gameui->mainmenu);


    // Main
    // -----------------------------------------------------------------------------------------
    gameui->mainmenu->pushChildFront(new idkui::Title("Paused", meatui::title_style));

    gameui->mainmenu->pushChildFront(new idkui::Button("Continue", meatui::button_style,
        [&events]()
        {
            events.mouseCapture(true);
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui::Button("Multiplayer", meatui::button_style,
        [gameui]()
        {
            gameui->ingame_menu->m_children[1][1] = gameui->multiplayer;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui::Button("Settings", meatui::button_style,
        [gameui]()
        {
            gameui->ingame_menu->m_children[1][1] = gameui->settings;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui::Button("Exit", meatui::button_style,
        []()
        {
            idk::ECS2::load("scenes/mainmenu");
        }
    ));
    // -----------------------------------------------------------------------------------------




    // Multiplayer
    // -----------------------------------------------------------------------------------------
    // gameui->multiplayer->pushChildFront(new idkui::Title("Multiplayer", meatui::title_style));

    // auto *splitwindow = new idkui::Split("", meatui::splitwindow_style, 0.35f);
    // multiplayer_list->pushChildFront(splitwindow);


    // splitwindow->setLeft(new idkui::TextInput(
    //     "hostname", meatui::textinput_style, gamedata->meatnet_hostname
    // ));

    // splitwindow->setRight(new idkui::Button("Connect", meatui::button_style,
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


    // multiplayer_list->pushChildBack(new idkui::Button("Return", meatui::button_style,
    //     [gameui]()
    //     {
    //         gameui->root->m_children[1][1] = nullptr;
    //     }
    // ));
    // -----------------------------------------------------------------------------------------



}



