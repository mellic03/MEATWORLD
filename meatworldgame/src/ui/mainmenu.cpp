#include "ui.hpp"


void
createMainMenu( idk::EngineAPI &api, idkui2::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI *gameui = &(gamedata->gameui);
    
    auto *&root = gameui->mainmenu_root;

    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();


    root = new idkui2::Grid("Root", meatui::root_style, 3, 4);
    root->setRowProportions({0.1, 0.8, 0.1});
    root->setColProportions({0.25, 0.5, 0.125, 0.125});

    gameui->mainmenu = new idkui2::List("Main Menu", meatui::list_style);
    root->setChild(1, 1, gameui->mainmenu);


    // Main
    // -----------------------------------------------------------------------------------------
    gameui->mainmenu->pushChildFront(new idkui2::Title("MEATWORLD", meatui::title_style));

    gameui->mainmenu->pushChildFront(new idkui2::Button("New Game", meatui::button_style,
        []()
        {
            idk::ECS2::load("scenes/a1m1");
            // events.mouseCapture(true);
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Continue", meatui::button_style,
        [gameui]()
        {
            // gameui->mainmenu_root->m_children[1][1] = gameui->multiplayer;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Multiplayer", meatui::button_style,
        [gameui]()
        {
            // gameui->mainmenu_root->m_children[1][1] = gameui->settings;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Settings", meatui::button_style,
        [gameui]()
        {
            // gameui->mainmenu_root->m_children[1][1] = gameui->settings;
        }
    ));

    gameui->mainmenu->pushChildFront(new idkui2::Button("Exit", meatui::button_style,
        [&engine]()
        {
            engine.shutdown();
        }
    ));
    // -----------------------------------------------------------------------------------------


}



