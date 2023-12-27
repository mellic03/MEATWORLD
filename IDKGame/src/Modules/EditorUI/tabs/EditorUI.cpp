#include "EditorUI-tabs.hpp"



bool
EditorUI::splitWindow_begin( const std::string &label )
{
    if (ImGui::BeginTable(label.c_str(), 2, EditorTab::resizeable_table_flags))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::BeginChild("left");

        return true;
    }

    return false;
}


void
EditorUI::splitWindow_split()
{
    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
    ImGui::TableNextColumn();

    ImGui::BeginChild("right");
}


void
EditorUI::splitWindow_end()
{
    ImGui::EndChild();
    ImGui::EndTable();
    ImGui::PopStyleVar();
}




