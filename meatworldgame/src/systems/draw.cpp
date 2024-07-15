#include "draw.hpp"
#include "sys-building.hpp"
#include "sys-player.hpp"

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



template <>
void meatworld::drawComponent<meatworld::WallCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &cmp = idk::ECS2::getComponent<meatworld::WallCmp>(obj_id);

    // std::string label = "Model " ICON_FA_DOWNLOAD;

    // ImGui::ButtonEx(label.c_str(), ImVec2(250, 50), ImGuiButtonFlags_None);

    // if (ImGui::BeginDragDropTarget())
    // {
    //     if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
    //     {
    //         std::string filepath(reinterpret_cast<char *>(payload->Data));
    //         cmp.model = ren.loadModel(filepath);
    //     }
    //     ImGui::EndDragDropTarget();
    // }

}
