#include "ingame-ui.hpp"

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKIO/IDKIO.hpp>

#include "../world.hpp"
#include "../../npc/drone.hpp"


// meatui::DroneMenu::DroneMenu()
// : idkui::Grid(5, 5, meatui::button_style)
// {

// }


// void
// meatui::DroneMenu::open( meat::CharacterDrone *V )
// {
//     this->current = V;
// }


// meatui::DroneMenu*
// meatui::vehicle_menu( idk::EngineAPI &api, meat::World &world )
// {
//     auto &engine = api.getEngine();
//     auto &ren    = api.getRenderer();
//     auto &UI     = world.ui;
//     auto &EM     = UI.EM;

//     auto btn = [&EM]( std::string label, const std::function<void()> &callback )
//     {
//         return new idkui::Button(label, button_style, buttontext_style, callback);
//     };

//     auto *menu = new meatui::DroneMenu;

//     menu->children[1] = btn("+1",  [menu](){ menu->current->addTargetHeight(1);  });
//     menu->children[2] = btn("+5",  [menu](){ menu->current->addTargetHeight(5);  });
//     menu->children[3] = btn("+20", [menu](){ menu->current->addTargetHeight(20); });

//     return menu;
// }



// void
// meatui::open_vehicle_menu( meat::World &world, meat::CharacterDrone *V )
// {
//     auto &UI = world.ui;
//     auto &EM = UI.EM;

//     UI.ingame_drone_menu->open(V);
//     EM.emit("menu-open", UI.ingame_drone_menu);
// }
