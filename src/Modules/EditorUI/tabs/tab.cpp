#include "../idk_EditorUI.hpp"
#include "EditorUI-tabs.hpp"


void
EditorUI_Module::_tab( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    ImGui::Begin("Editor UI");
    ImGui::DockSpace(ImGui::GetID("Editor DockSpace"));

    this->_tab_gameobjects(api);
    this->_tab_inspect(api, m_selected_object);

    EditorTab::shader_programs(api);
    EditorTab::skybox(api);
    EditorTab::cameras(api);

    ImGui::End();
}
