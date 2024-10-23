#include "ingame-ui.hpp"
#include "ui-elements.hpp"
#include "../world.hpp"

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
// #include <IDKIO/IDKIO.hpp>


idkui::Element*
meatui::inventory_ui( idk::EngineAPI &api, meat::World &world )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &UI     = world.ui;
    auto &EM     = UI.EM;

    auto btn = [&EM]( std::string label )
    {
        return new Button(label, [](){});
    };

    auto *menu = new idkui::GridUniform(8*8, 12, grid_style);
    auto *win  = new Window(glm::vec3(0.0f), world);
          win->setDepthOffset(2);

    win->pushChild(
        new meatui::Menu("Some Window", {
        btn("Button A"),
        btn("Button B"),
        btn("Button C"),
        btn("Button D"),
        btn("Button E")
        })
    );

    menu->children[0] = win;

    for (int i=1; i<8*8; i++)
    {
        menu->children[i] = new GridSquare(UI.textures[(i)%4]);
    }

    return menu;
}
