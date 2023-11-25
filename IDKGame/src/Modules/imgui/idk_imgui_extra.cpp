#include "idk_imgui_extra.hpp"

void idkgui::dragVec3( std::string name, float *data, float min, float max,
                           float speed, const char *format, float default_value )
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

    ImGui::BeginTable(name.c_str(), 4);
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


