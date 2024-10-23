#pragma once
#include <IDKGraphics/UI/idk_ui.hpp>

namespace idk  { class EngineAPI; }
namespace meat { class World;     }


namespace meatui
{
    idkui::Element *ingame_menu( idk::EngineAPI&, meat::World& );
    idkui::Element *settings_menu( idk::EngineAPI&, meat::World& );

}

