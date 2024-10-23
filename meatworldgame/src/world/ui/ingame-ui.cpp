#include "menu-ui.hpp"
#include "ui-elements.hpp"
#include "../world.hpp"

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/idk_render_settings.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKIO/IDKIO.hpp>



idkui::Element*
meatui::ingame_ui( idk::EngineAPI &api, meat::World &world )
{
    auto btn = []( std::string label )
    {
        return new Button(label, [](){});
    };

    auto *img  = new idkui::Image(world.ui.textures[0], grid_style);
    auto *grid = new idkui::Grid(7, 5, meatui::root_style);

    grid->getChild(0, 0) = img;
    grid->getChild(0, 1) = btn("1");
    grid->getChild(0, 2) = btn("2");
    grid->getChild(0, 3) = btn("3");
    grid->getChild(0, 4) = btn("4");


    return grid;
}


