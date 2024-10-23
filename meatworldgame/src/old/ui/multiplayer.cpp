#include "ui.hpp"
#include "../systems/sys-player.hpp"


void
createMultiplayer( idk::EngineAPI &api, idkui::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI       *gameui       = &(gamedata->gameui);
    meatworld::GameSettings *gamesettings = &(gamedata->settings);

    gameui->multiplayer = new idkui::List("Multiplayer", meatui::list_style);

    // Main
    // -----------------------------------------------------------------------------------------
    {
        gameui->multiplayer->pushChildFront(new idkui::Title("Multiplayer", meatui::title_style));


        auto *connect = new idkui::Button("Connect", meatui::button_style,
            [gamedata]()
            {
                meatworld::PlayerSys::init_multiplayer(gamedata->mp_hostname);
            }
        );



        auto *split = new idkui::Split("", meatui::splitwindow_style, 0.65f); 
        gameui->multiplayer->pushChildFront(split);

        split->setLeft(
            new idkui::TextInput("", meatui::textinput_style, gamedata->mp_hostname)
        );

        split->setRight(connect);



        gameui->multiplayer->pushChildBack(new idkui::Button("Return", meatui::button_style,
            [gameui]()
            {
                gameui->root->m_children[1][1] = nullptr;
            }
        ));
    }
    // -----------------------------------------------------------------------------------------

}



