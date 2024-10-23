#include "draw.hpp"
#include "sys-building.hpp"
#include "sys-player.hpp"
#include "sys-vehicle.hpp"
#include "sys-actor.hpp"

#include "../character/base.hpp"
#include "../vehicle/vehicle.hpp"
#include "../character/weapon.hpp"

#include <IDKBuiltinUI/EditorUI.hpp>
#include <IDKBuiltinUI/common/idk_imgui_extra.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>




void drag_drop_thing( std::string label, const std::string &payloadname, int &obj_id )
{
    label = label + " " + std::string(ICON_FA_DOWNLOAD);

    if (obj_id != -1)
    {
        if (idk::ECS2::gameObjectExists(obj_id))
        {
            label = idk::ECS2::getGameObjectName(obj_id);
        }

        else
        {
            obj_id = -1;
        }
    }

    ImGui::ButtonEx(label.c_str(), ImVec2(250, 50), ImGuiButtonFlags_None);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            obj_id = *reinterpret_cast<int *>(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
}



void
meatworld::drawComponent_CharacterCmp( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();
    auto &cmp = ECS2::getComponent<meatworld::CharacterCmp>(obj_id);

    ImGui::DragFloat("Walk speed", &cmp.walk_speed, 0.05f);
    ImGui::DragFloat("Run speed", &cmp.walk_speed, 0.05f);

}


void
meatworld::drawComponent_PlayerCmp( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();
    auto &cmp = ECS2::getComponent<meatworld::PlayerCmp>(obj_id);

}


void
meatworld::drawComponent_VehicleCmp( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();
    auto &cmp = ECS2::getComponent<meat::VehicleCmp>(obj_id);

    meat::VehicleBase *V = cmp.actor;
    auto &desc = V->getVehicleDesc();

    ImGui::InputFloat3("Spring",  &desc.spring.x);
    ImGui::InputFloat("Steer",    &desc.steer_spd);
    ImGui::InputFloat("Unsteer",  &desc.steer_damp);

}


void
meatworld::drawComponent_ActorCmp( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();
    auto &cmp = ECS2::getComponent<meat::ActorCmp>(obj_id);

    meat::ActorBase *C = cmp.actor;
    meat::ActorDesc adesc = C->getActorDesc();

    bool grounded = C->isGrounded();
    ImGui::Checkbox("Grounded", &grounded);
    ImGui::Separator();

    ImGui::InputFloat("Height",      &adesc.height);
    ImGui::InputFloat("Walk speed",  &adesc.walk_spd);
    ImGui::InputFloat("Run speed",   &adesc.run_spd);
    ImGui::Separator();

    ImGui::InputFloat("mass",        &adesc.mass);
    ImGui::InputFloat("buoyancy",    &adesc.buoyancy);
    ImGui::Separator();

    ImGui::DragFloat3("Acc", &(C->getAcc().x));
    ImGui::DragFloat3("Vel", &(C->getVelocity().x));
    ImGui::Separator();

    ImGui::InputFloat("Imp damp", &adesc.imp_damp);
    ImGui::InputFloat("Acc damp", &adesc.acc_damp);
    ImGui::InputFloat("Vel damp", &adesc.vel_damp);
    ImGui::Separator();

    ImGui::InputFloat("Acc max",  &adesc.acc_max);
    ImGui::InputFloat("Vel max",  &adesc.vel_max);
    ImGui::Separator();

    ImGui::InputFloat("Jump imp",  &adesc.jmp_imp);
    ImGui::InputFloat("Jump acc",  &adesc.jmp_acc);
    ImGui::InputFloat("Jump dur",  &adesc.jmp_dur);

    C->setActorDesc(adesc);

}


void
meatworld::drawComponent_WeaponRangedCmp( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &ren = api.getRenderer();
    auto &cmp = ECS2::getComponent<meat::WeaponRangedCmp>(obj_id);

    meat::WeaponRanged *W = cmp.weapon;
    auto &desc = W->getRangedDesc();

    ImGui::InputFloat("Velocity",      &desc.vel);
    ImGui::InputFloat("Velocity RNG",  &desc.vel_rng);

    ImGui::InputFloat("Mass",          &desc.mass);
    ImGui::InputFloat("Mass RNG",      &desc.mass_rng);

    ImGui::InputFloat("Spread",        &desc.spread);
    ImGui::InputFloat("Spread RNG",    &desc.spread_rng);

    ImGui::InputFloat("Cooldown",      &desc.cooldown);
    ImGui::InputFloat("Cooldown RNG",  &desc.cooldown_rng);

}