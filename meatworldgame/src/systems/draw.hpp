#pragma once

namespace idk
{
    class EngineAPI;
}


namespace meatworld
{
    void drawComponent_PlayerCmp( idk::EngineAPI&, int obj_id );
    void drawComponent_CharacterCmp( idk::EngineAPI&, int obj_id );
    void drawComponent_VehicleCmp( idk::EngineAPI&, int obj_id );
    void drawComponent_ActorCmp( idk::EngineAPI&, int obj_id );
    void drawComponent_WeaponRangedCmp( idk::EngineAPI&, int obj_id );
}
