#include "../idk_EditorUI.hpp"


void
EditorUI_Module::_menubar( idk::Engine &engine )
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL + N"))
            {

            }

            if (ImGui::MenuItem(IDK_ICON_SAVE " Save", "CTRL + S"))
            {

            }

            if (ImGui::MenuItem(IDK_ICON_SAVE_AS " Save As", "CTRL + SHFT + S"))
            {
                std::string selection;
                static bool open = true;
                EditorUI::fileSelectPopup("./", selection, open);
            }

            if (ImGui::MenuItem("Load", "CTRL + L"))
            {
                
            }

            ImGui::Separator();
            ImGui::EndMenu();
        }

        _menubar_settings(engine);

        if (ImGui::BeginMenu("Scene"))
        {
            ImGui::EndMenu();
        }

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("Framerate: %f", fps);

        ImGui::EndMainMenuBar();
    }
}

