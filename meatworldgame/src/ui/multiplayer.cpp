#include "ui.hpp"



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


        gameui->multiplayer->pushChildFront(new idkui2::Button("Connect", meatui::button_style,
            [gamedata]()
            {
                auto callback = [gamedata]( std::string filepath )
                {
                    gamedata->init_multiplayer(filepath);
                };

                gamedata->meatnet = new meatnet::Client(
                    "Michael", gamedata->meatnet_hostname, 4200, callback
                );
            }
        ));


        gameui->multiplayer->pushChildBack(new idkui2::Button("Return", meatui::button_style,
            [gameui]()
            {
                gameui->root->m_children[1][1] = nullptr;
            }
        ));
    }
    // -----------------------------------------------------------------------------------------

}



