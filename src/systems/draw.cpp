#include "sys-player.hpp"

#include <IDKBuiltinUI/EditorUI.hpp>
#include <IDKBuiltinUI/common/idk_imgui_extra.hpp>

#include <IDKGameEngine/IDKGameEngine.hpp>


template <>
void
idk::ECS2::userCallback<idk::PlayerControllerCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<idk::PlayerControllerCmp>(obj_id);

    ImGui::DragFloat("Walk speed", &cmp.walk_speed, 0.0f, 16.0f);
    ImGui::DragFloat("Run speed",  &cmp.run_speed,  0.0f, 16.0f);
    ImGui::DragFloat("Jump force", &cmp.jump_force, 0.0f, 16.0f);
}



template <>
void
idk::ECS2::userCallback<idk::OLPlayerControllerCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<idk::OLPlayerControllerCmp>(obj_id);

}




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


template <>
void
idk::ECS2::userCallback<idk::ArmCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<idk::ArmCmp>(obj_id);

    ImGui::Checkbox("is left", &cmp.is_left);
    ImGui::DragFloat("stride", &cmp.stride,      0.5f, 5.0f);
    ImGui::DragFloat("speed",  &cmp.step_speed,  2.0f, 16.0f);
    ImGui::DragFloat("height", &cmp.step_height, 0.1f, 1.0f);
    ImGui::DragFloat("distAB", &cmp.distAB,      0.1f, 1.0f);
    ImGui::DragFloat("distBC", &cmp.distBC,      0.1f, 1.0f);

    drag_drop_thing( "Partner Limb", "SCENE_HIERARCHY", cmp.partner_id);
    drag_drop_thing( "Rest Target",  "SCENE_HIERARCHY", cmp.rest_target);
    drag_drop_thing( "Ray Target",   "SCENE_HIERARCHY", cmp.ray_target);
    drag_drop_thing( "Pole Target",  "SCENE_HIERARCHY", cmp.pole_target);
}





void idkg::registeruserCallbacks()
{
    // idk::ECS2::getComponentArray<idk::PlayerControllerCmp>().setuserCallback(
    //     game_drawComponent<idk::PlayerControllerCmp>
    // );

    // idk::ECS2::getComponentArray<idk::ArmCmp>().setuserCallback(
    //     game_drawComponent<idk::ArmCmp>
    // );

}
