#pragma once

#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKECS/IDKECS.hpp>

#include "../../../meatnet/src/meatnet.hpp"
#include "../gamedata.hpp"


void createMenu( idk::EngineAPI &api,  idkui2::LayoutManager *LM,
                 meatnet::Host *&host, meatnet::Client *&client,
                 meatworld::GameData*, meatworld::GameUI* );


void creatMultiplayer ( idk::EngineAPI&,  idkui2::LayoutManager*, meatworld::GameUI* );
void createSettings   ( idk::EngineAPI&,  idkui2::LayoutManager*, meatworld::GameUI* );
void createSyndromes  ( idk::EngineAPI&,  idkui2::LayoutManager*, meatworld::GameUI* );

