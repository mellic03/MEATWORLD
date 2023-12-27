#include "../idk_EditorUI.hpp"


void
EditorUI_Module::_menubar_settings( idk::Engine &engine )
{
    if (ImGui::BeginMenu("Settings"))
    {
        if (ImGui::BeginMenu("Font Size"))
        {
            for (int i=0; i<NUM_FONTS; i++)
            {
                int font_size = FONT_SIZES[i];
                std::string label = std::to_string(font_size) + "px";

                if (ImGui::MenuItem(label.c_str()))
                {
                    ImGui::GetIO().FontDefault = m_fonts[i];
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("ImGui Demo Window"))
        {
            m_show_ImGui_demo = true;
        }

        ImGui::EndMenu();
    }
}

