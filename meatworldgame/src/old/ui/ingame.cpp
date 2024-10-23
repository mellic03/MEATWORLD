#include "ui.hpp"

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>





void
createInGame( idk::EngineAPI &api, idkui::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI *gameui = &(gamedata->gameui);

    gameui->ingame_ui = new idkui::Grid("InGameUI", meatui::root_style, 5, 5);

    auto *&root = gameui->ingame_ui;
    root->setChild(2, 2, new idkui::Label("[E] Drive vehicle", meatui::label_style));

}
