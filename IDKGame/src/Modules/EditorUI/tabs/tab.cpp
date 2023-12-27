#include "../idk_EditorUI.hpp"
#include "EditorUI-tabs.hpp"


void
EditorUI_Module::_tab( idk::Engine &engine )
{
    ImGui::Begin("Editor UI");
    ImGui::DockSpace(ImGui::GetID("Editor DockSpace"));

    this->_tab_gameobjects(engine);
    this->_tab_inspect(engine, m_selected_object);

    EditorTab::shader_programs(engine);
    EditorTab::skybox(engine);
    EditorTab::cameras(engine);

    ImGui::End();
}
