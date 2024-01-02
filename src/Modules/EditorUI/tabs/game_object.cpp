#include "../idk_EditorUI.hpp"

#include "EditorUI-tabs.hpp"
#include <ComponentSystems/idk_transform_CS.h>


void
EditorUI_Module::_tab_gameobjects( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    auto &tCS = engine.getCS<Transform_CS>();
    auto &objects = engine.gameObjects();

    ImGui::Begin("Game Objects");

    ImGui::BeginChild("split-window", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

    if (EditorUI::splitWindow_begin("Game Objects"))
    {
        for (int id: engine.gameObjects())
        {
            std::string name = std::to_string(id);

            bool enabled = id == m_selected_object;

            if (ImGui::MenuItem(name.c_str(), "", &enabled))
            {
                m_selected_object = id;
            }
        }

        EditorUI::splitWindow_split();

        ImGui::Text("Name: ");

        EditorUI::splitWindow_end();
    }
    ImGui::EndChild();


    if (ImGui::Button(IDK_ICON_PLUS " add"))
    {
        engine.createGameObject();
        // engine.gameObjects().print();
    }

    ImGui::SameLine();

    if (ImGui::Button(IDK_ICON_TRASH " delete"))
    {
        engine.deleteGameObject(m_selected_object);
        std::cout << "Deleted object " << m_selected_object << "\n";
        m_selected_object = 0;

        // engine.gameObjects().print();
    }


    ImGui::End();
}

