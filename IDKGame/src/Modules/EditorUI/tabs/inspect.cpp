#include "../idk_EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include "ComponentSystems/componentsystems.hpp"


static void
tab_inspect_CS( Transform_CS &CS, int object_id )
{
    EditorUI::dragVec3(
        "Position",
        CS.getTransform(object_id).position_ptr(),
        0.0f, 0.0f, 0.01f, "%.2f", 0.0f
    );
}



static void
tab_inspect_CS( Model_CS &CS, int object_id )
{
    idk::Model &model = CS.getModel(object_id);

    for (auto &mesh: model.meshes)
    {
        ImGui::Text(IDK_ICON_EYE " Mesh %d", mesh.material_id);
        ImGui::Text(IDK_ICON_PERSON " Mesh %d", mesh.material_id);
        ImGui::Text(IDK_ICON_EYE_CLOSED " Mesh %d", mesh.material_id);
        // auto &material = engineptr
        // mesh.material_id
    }


    // EditorUI::dragVec3(
    //     "Position",
    //     CS.getTransform(object_id).position_ptr(),
    //     0.0f, 0.0f, 0.01f, "%.2f", 0.0f
    // );

}




static void
tab_inspect_component( idk::Engine &engine, int object_id, idk::ComponentSystem *CS )
{
    if (CS == nullptr)
    {
        return;
    }

    int id = CS->id();

    std::string label = CS->name() + " component";
    ImGui::BeginChild(label.c_str());


    if (CS == &engine.getCS<Transform_CS>())
    {
        tab_inspect_CS(*dynamic_cast<Transform_CS *>(CS), object_id);
    }

    if (CS == &engine.getCS<Model_CS>())
    {
        tab_inspect_CS(*dynamic_cast<Model_CS *>(CS), object_id);
    }

    ImGui::EndChild();

    // if (id == engine.getCSID("terrain"))
    // {

    // }

    // if (id == engine.getCSID("control"))
    // {

    // }
}



void
EditorUI_Module::_tab_inspect( idk::Engine &engine, int object_id )
{
    ImGui::Begin("Inspect");

    if (object_id == -1)
    {
        ImGui::End();
        return;
    }

    std::string label = "Object ID: " + std::to_string(object_id);
    ImGui::Text(label.c_str());

    static idk::ComponentSystem *CS_ptr = nullptr;
    static int stored_id = 0;

    if (object_id != stored_id)
    {
        stored_id = object_id;
        CS_ptr    = nullptr;
    }


    if (EditorUI::splitWindow_begin("Inspect Component"))
    {
        for (auto &CS: engine.getComponentSystems())
        {
            const std::string &name = CS->name();
            const int CS_id = CS->id();

            if (engine.hasComponent(object_id, CS_id))
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    CS_ptr = CS;
                }
            }
        }

        EditorUI::splitWindow_split();
        tab_inspect_component(engine, object_id, CS_ptr);
        EditorUI::splitWindow_end();
    }



    ImGui::End();
}


