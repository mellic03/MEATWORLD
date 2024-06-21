#pragma once

#include <IDKGraphics/UI/idk_ui2.hpp>
#include "../../../meatnet/src/meatnet.hpp"

idkui2::Panel *createMenu( idk::EngineAPI &api,  idkui2::LayoutManager *LM,
                           meatnet::Host *&host, meatnet::Client *&client );


