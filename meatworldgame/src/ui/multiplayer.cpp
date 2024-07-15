#include "ui.hpp"
#include "../systems/sys-player.hpp"


void
createMultiplayer( idk::EngineAPI &api, idkui2::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI       *gameui       = &(gamedata->gameui);
    meatworld::GameSettings *gamesettings = &(gamedata->settings);

    gameui->multiplayer = new idkui2::List("Multiplayer", meatui::list_style);

    // Main
    // -----------------------------------------------------------------------------------------
    {
        gameui->multiplayer->pushChildFront(new idkui2::Title("Multiplayer", meatui::title_style));


        auto *connect = new idkui2::Button("Connect", meatui::button_style,
            [gamedata]()
            {
                meatworld::PlayerSys::init_multiplayer(gamedata->mp_hostname);
            }
        );



        auto *split = new idkui2::Split("", meatui::splitwindow_style, 0.65f); 
        gameui->multiplayer->pushChildFront(split);

        split->setLeft(
            new idkui2::TextInput("", meatui::textinput_style, gamedata->mp_hostname)
        );

        split->setRight(connect);



        gameui->multiplayer->pushChildBack(new idkui2::Button("Return", meatui::button_style,
            [gameui]()
            {
                gameui->root->m_children[1][1] = nullptr;
            }
        ));
    }
    // -----------------------------------------------------------------------------------------

}



