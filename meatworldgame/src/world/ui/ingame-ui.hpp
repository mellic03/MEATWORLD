#pragma once

#include <IDKGraphics/UI/idk_ui.hpp>

namespace idk  { class EngineAPI; }
namespace meat { class World;     }


namespace meatui
{
    idkui::Element *ingame_ui( idk::EngineAPI&, meat::World& );
    idkui::Element *inventory_ui( idk::EngineAPI&, meat::World& );
}


