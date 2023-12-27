#include "idk_imgui_extra.hpp"
#include "../icon-headers/idk_Icons.hpp"

#include <filesystem>

void
EditorUI::dragVec3( std::string name, float *data, float min, float max,
                    float speed, const char *format, float default_value )
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

    ImGui::BeginTable(name.c_str(), 4, ImGuiTableFlags_SizingStretchSame);
    {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text(name.c_str());

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

            float line_height = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 button_size = { line_height + 3.0f, line_height };

            ImGui::TableNextColumn();
            if (ImGui::Button("X", button_size))
                data[0] = default_value;
            ImGui::SameLine();
            ImGui::DragFloat("##X", &data[0], speed, min, max, format);
            
            ImGui::TableNextColumn();
            if (ImGui::Button("Y", button_size))
                data[1] = default_value;
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &data[1], speed, min, max, format);

            ImGui::TableNextColumn();
            if (ImGui::Button("Z", button_size))
                data[2] = default_value;
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &data[2], speed, min, max, format);

        ImGui::PopStyleVar();

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
}



static void
file_select( const std::string &path, std::string &selection )
{
    using namespace std::filesystem;

    static const auto dir_flags  = ImGuiTreeNodeFlags_SpanAvailWidth;
    static const auto file_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

    for (auto &dir_entry: directory_iterator(path))
    {
        if (dir_entry.is_directory())
        {
            std::string entry_path = dir_entry.path();
            std::string entry_name = dir_entry.path().filename();
            std::string label = IDK_ICON_FOLDER " " + entry_name;

            if (ImGui::TreeNodeEx(label.c_str(), dir_flags))
            {
                file_select(entry_path, selection);
                ImGui::TreePop();
            }
        }
    }


    for (auto &dir_entry: directory_iterator(path))
    {
        std::string entry_name = dir_entry.path().filename();

        if (dir_entry.is_directory() == false)
        {
            std::string entry_name = dir_entry.path().filename();
            std::string label = IDK_ICON_FILE " " + entry_name;

            if (ImGui::TreeNodeEx(label.c_str(), file_flags))
            {
                ImGui::TreePop();
            }
        }
    }

}


void
EditorUI::fileSelectPopup( const std::string &entry_path, std::string &selection, bool &open )
{
    open = true;

    if (ImGui::Begin("File Selection", &open, 0))
    {
        ImGui::BeginChild("selection-window", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        file_select( entry_path, selection );
        ImGui::EndChild();

        if (ImGui::Button("Close"))
        {
            open = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Open"))
        {
            open = false;
        }

        ImGui::End();
    }

}


