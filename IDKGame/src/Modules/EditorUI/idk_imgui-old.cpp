// #include "idk_imgui.hpp"
// #include "idk_imgui_extra.hpp"

// #include "../../ComponentSystems/componentsystems.hpp"


// SDL_Window *m2_SDL_window;
// SDL_GLContext m2_SDL_GL_Context;

// void
// EditorUI_Module::init( idk::Engine &engine )
// {
//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

//     ImGui::StyleColorsDark();

//     ImGui_ImplSDL2_InitForOpenGL(
//         engine.rengine().SDLWindow(),
//         engine.rengine().SDLGLContext()
//     );

//     ImGui_ImplOpenGL3_Init("#version 440");

//     engine.eventManager().onSDLPollEvent(
//         [](SDL_Event *event)
//         {
//             ImGui_ImplSDL2_ProcessEvent(event);
//         }
//     );

// }


// void
// EditorUI_Module::f_settings_graphics( idk::Engine &engine )
// {
//     idk::RenderEngine &ren = engine.rengine();

//     if (ImGui::BeginChild("Settings_Graphics"))
//     {
//         ImGui::BeginChild("Grouping", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

//         std::map<std::string, idk::glShader> &programs = ren.getPrograms();

//         static int selected = 0;
//         static std::string selected_name = "";
//         int n = 0;

//         ImGuiStyle& style = ImGui::GetStyle();
//         float left_w = 0.25 * ImGui::GetContentRegionAvail().x;
//         float right_w = ImGui::GetContentRegionAvail().x - left_w;
//         ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);

//         ImGui::BeginChild(child_id, ImVec2(left_w, 0), true, 0);
//             for (auto &[name, program]: programs)
//             {
//                 auto &definitions = program.getDefinitions();

//                 if (ImGui::Selectable(name.c_str(), selected == n))
//                 {
//                     selected = n;
//                     selected_name = name;
//                 }
//                 n += 1;
//             }
//         ImGui::EndChild();

//         ImGui::SameLine();

//         child_id = ImGui::GetID((void*)(intptr_t)1);
//         ImGui::BeginChild(child_id, ImVec2(right_w, 0), true, 0);
//             child_id = ImGui::GetID((void*)(intptr_t)2);
//             ImGui::BeginChild("Defintions", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
//                 if (selected_name != "")
//                 {
//                     for (auto &[name, def]: ren.getProgram(selected_name).getDefinitions())
//                     {
//                         ImGui::InputText(name.c_str(), &def.value);
//                     }
//                 }
//             ImGui::EndChild();

//             if (ImGui::Button("Recompile"))
//             {
//                 ren.getProgram(selected_name).compile();
//             }
//         ImGui::EndChild();
//         ImGui::EndChild();

//         if (ImGui::Button("Close"))
//         {
//             m_menu_action = "";
//         }

//         ImGui::EndChild();
//     }
// }


// void
// EditorUI_Module::f_settings_camera( idk::Engine &engine )
// {
//     if (ImGui::BeginChild("Settings_Camera"))
//     {
//         idk::Camera &cam = engine.rengine().getCamera();

//         // static glm::vec3 offset(0.5f, 1.0f, 1.0f);
//         // idkgui::dragVec3("Offset", &offset[0], -2.0f, 2.0f, 0.1f, "%.1f", 0.0f);
//         // cam.setOffset(offset);

//         ImGui::Spacing();

//         ImGui::SliderFloat("Strength", &cam.m_abr_str, 0.0f, 0.1f, "%.4f");
//         ImGui::SliderFloat2("R",  &cam.m_r_abr.x,  -1.0f, 1.0f, "%.0f");
//         ImGui::SameLine();
//         if (ImGui::Button("RReset"))
//             cam.m_r_abr = glm::vec2(0.0f);

//         ImGui::SliderFloat2("G",  &cam.m_g_abr.x,  -1.0f, 1.0f, "%.1f");
//         ImGui::SameLine();
//         if (ImGui::Button("GReset"))
//             cam.m_g_abr = glm::vec2(0.0f);

//         ImGui::SliderFloat2("B",  &cam.m_b_abr.x,  -1.0f, 1.0f, "%.1f");
//         ImGui::SameLine();
//         if (ImGui::Button("BReset"))
//             cam.m_b_abr = glm::vec2(0.0f);

//         ImGui::Spacing();

//         ImGui::SliderFloat("Bloom", &cam.m_bloom_gamma.x, 0.0f, 1.0f, "%.2f");
//         ImGui::SliderFloat("Gamma", &cam.m_bloom_gamma.y, 0.0f, 4.0f, "%.2f");
//         ImGui::Spacing();

//         ImGui::SliderFloat("Adjust Rate",  &cam.m_bloom_gamma.z,  15.0f,  1024.0f, "%1.0f");
//         ImGui::Spacing();

//         if (ImGui::Button("Cancel"))
//         {
//             m_menu_action = "";
//         }

//         ImGui::EndChild();
//     }
// }



// void
// EditorUI_Module::f_settings_pointlight( idk::Engine &engine )
// {
//     idk::RenderEngine &ren = engine.rengine();


//     if (ImGui::BeginChild("Settings_Pointlight"))
//     {
//         ImGui::BeginChild("REE", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

//         static int selected = 0;
//         int n = 0;

//         ImGuiStyle& style = ImGui::GetStyle();
//         float left_w = 0.25 * ImGui::GetContentRegionAvail().x;
//         float right_w = ImGui::GetContentRegionAvail().x - left_w;
//         ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);

//         ImGui::BeginChild(child_id, ImVec2(left_w, 0), true, 0);
//             for (int i=0; i<ren.lightSystem().pointlights().size(); i++)
//             {
//                 if (ImGui::Selectable(std::to_string(i).c_str(), selected == n))
//                 {
//                     selected = n;
//                 }
//                 n += 1;
//             }

//             if (ImGui::Button("Add"))
//             {
//                 ren.lightSystem().createPointlight();
//             }
//         ImGui::EndChild();

//         ImGui::SameLine();

//         child_id = ImGui::GetID((void*)(intptr_t)1);

//         ImGui::BeginChild(child_id, ImVec2(right_w, 0), true, 0);

//             child_id = ImGui::GetID((void*)(intptr_t)2);
    
//             ImGui::BeginChild("REEEE", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
//                 auto &light = ren.lightSystem().pointlights()[selected];

//                 ImGui::ColorEdit3("Ambient", &light.ambient[0], ImGuiColorEditFlags_HDR);
//                 ImGui::ColorEdit3("Diffuse", &light.diffuse[0], ImGuiColorEditFlags_HDR);

//                 ImGui::DragFloat("Constant",  &light.attenuation[0], 0.1f, 0.0f, 1.0f);
//                 ImGui::DragFloat("Linear",    &light.attenuation[1], 0.1f, 0.0f, 1.0f);
//                 ImGui::DragFloat("Quadratic", &light.attenuation[2], 0.1f, 0.0f, 1.0f);

//                 idkgui::dragVec3("Position", &light.position[0], 0.0f, 0.0f, 0.1f, "%0.2f", 0.0f);

//                 if (ImGui::Button("Cancel"))
//                 {
//                     m_menu_action = "";
//                 }

//             ImGui::EndChild();

//         if (ImGui::Button("Remove"))
//         {
//             ren.lightSystem().destroyPointlight(selected);
//             selected = 0;
//         }
//         ImGui::EndChild();
//         ImGui::EndChild();

//         if (ImGui::Button("Close"))
//         {
//             m_menu_action = "";
//         }

//         ImGui::EndChild();
//     }
// }



// void
// EditorUI_Module::f_settings_skybox( idk::Engine &engine )
// {
//     idk::RenderEngine &ren = engine.rengine();

//     if (ImGui::BeginChild("Settings_Skybox"))
//     {
//         static int selected = 0;
//         ren.current_skybox = selected;

//         ImGuiStyle& style = ImGui::GetStyle();
//         float left_w = 0.25 * ImGui::GetContentRegionAvail().x;
//         float right_w = ImGui::GetContentRegionAvail().x - left_w;
//         ImGuiID child_id = ImGui::GetID((void*)(intptr_t)52);

//         for (int i=0; i<ren.skyboxes.size(); i++)
//         {
//             if (ImGui::Selectable(std::to_string(i).c_str(), selected == i))
//             {
//                 selected = i;
//             }
//         }

//         if (ImGui::Button("Cancel"))
//         {
//             m_menu_action = "";
//         }

//         ImGui::EndChild();
//     }

// }



// void
// EditorUI_Module::f_settings_objects( idk::Engine &engine )
// {
//     idk::RenderEngine &ren = engine.rengine();

//     if (ImGui::BeginChild("Settings_Objects"))
//     {
//         const std::set<int> &obj_ids = engine.gameObjects();

//         static int selected = 0;

//         ImGuiStyle& style = ImGui::GetStyle();
//         float left_w = 0.2 * ImGui::GetContentRegionAvail().x;
//         float right_w = ImGui::GetContentRegionAvail().x - left_w;

//         ImGui::BeginChild("obj_select", ImVec2(left_w, 0), true, 0);
//         for (int id: obj_ids)
//         {
//             if (ImGui::Selectable(std::to_string(id).c_str(), selected == id))
//             {
//                 selected = id;
//             }
//         }
//         ImGui::EndChild();

//         ImGui::SameLine();

//         ImGui::BeginChild("obj_edit", ImVec2(right_w, 0), true, 0);
//         {
//             auto &tCS = engine.getCS<Transform_CS>("transform");
//             auto &mCS = engine.getCS<Model_CS>("model");

//             if (engine.hasComponent(selected, "transform"))
//             {
//                 glm::vec3 pos = tCS.getTransform(selected).position();
//                 idkgui::dragVec3("position", &pos[0], 0.0f, 0.0f, 0.1f, "%.1f", 0.0f);
//                 tCS.translate(selected, pos - tCS.getTransform(selected).position());


//                 glm::vec3 scale = tCS.getTransform(selected).scale();
//                 idkgui::dragVec3("scale", &scale[0], 0.0f, 0.0f, 0.1f, "%.1f", 0.0f);
//                 tCS.scale(selected, scale);
//             }

//             if (engine.hasComponent(selected, "model"))
//             {
//                 idk::Model &model = mCS.getModel(selected);

//                 // if (model.animated)
//                 // {
//                 //     static float alpha  = 0.5f;
//                 //     static int   anim_A = 0;
//                 //     static int   anim_B = 0;

//                 //     ImGui::DragFloat("Alpha", &alpha, 0.01f, 0.0f, 1.0f);

//                 //     ImGui::DragInt(
//                 //         "Animation A", &anim_A,
//                 //         0.1f, 0, model.m_anim_controller.numAnimations() - 1
//                 //     );

//                 //     ImGui::SameLine();

//                 //     ImGui::DragInt(
//                 //         "Animation B", &anim_B,
//                 //         0.1f, 0, model.m_anim_controller.numAnimations() - 1
//                 //     );


//                 //     model.m_anim_controller.setAnimA(anim_A);
//                 //     model.m_anim_controller.setAnimB(anim_B);
//                 //     model.m_anim_controller.setBlendAlpha(alpha);
//                 // }

//                 for (size_t i=0; i<model.meshes.size(); i++)
//                 {
//                     idk::Mesh &mesh = model.meshes[i];
//                     idk::Material &material = ren.modelSystem().getMaterials().get(mesh.material_id);

//                     ImGui::Text("Material: %s", material.name.c_str());

//                     ImGui::DragFloat(("normal" + std::to_string(i)).c_str(), &material.normal_strength, 0.01f, 0.0f, 1.0f);
//                     ImGui::DragFloat(("rough"  + std::to_string(i)).c_str(), &material.roughness_strength, 0.01f, 0.0f, 2.0f);
//                     ImGui::DragFloat(("metal"  + std::to_string(i)).c_str(), &material.metallic_strength, 0.01f, 0.0f, 2.0f);

//                     ImGui::DragFloat(
//                         ("displacement"  + std::to_string(i)).c_str(),
//                         &material.displacement_strength, 0.01f, 0.0f, 1.0f
//                     );

//                     ImGui::Dummy({0, 0});
//                 }
//             }

//             if (ImGui::Button("copy"))
//             {
//                 engine.createGameObject(selected);
//             }

//             ImGui::EndChild();
//         }


//         if (ImGui::Button("Cancel"))
//         {
//             m_menu_action = "";
//         }

//         ImGui::EndChild();
//     }

// }



// void
// EditorUI_Module::f_settings_dirlight( idk::Engine &engine )
// {
//     idk::RenderEngine &ren = engine.rengine();


//     if (ImGui::BeginChild("Settings_Dirlight"))
//     {
//         ImGui::BeginChild("REE", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

//         static int selected = -1;
//         int n = 0;

//         ImGuiStyle& style = ImGui::GetStyle();
//         float left_w = 0.25 * ImGui::GetContentRegionAvail().x;
//         float right_w = ImGui::GetContentRegionAvail().x - left_w;
//         ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);

//         ImGui::BeginChild(child_id, ImVec2(left_w, 0), true, 0);
//             for (int i=0; i<ren.lightSystem().dirlights().size(); i++)
//             {
//                 if (ImGui::Selectable(std::to_string(i).c_str(), selected == n))
//                 {
//                     selected = n;
//                 }
//                 n += 1;
//             }

//             if (ImGui::Button("Add"))
//             {
//                 ren.lightSystem().createDirlight();
//             }
//         ImGui::EndChild();

//         ImGui::SameLine();

//         child_id = ImGui::GetID((void*)(intptr_t)1);

//         ImGui::BeginChild(child_id, ImVec2(right_w, 0), true, 0);

//             child_id = ImGui::GetID((void*)(intptr_t)2);

//             if (selected >= 0)
//             {
//                 ImGui::BeginChild("REEEE", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

//                     auto &dirlight = ren.lightSystem().dirlights()[selected];

//                     // ImGui::Image(
//                     //     (void*)(intptr_t)(ren.lightSystem().depthCascade().depth_attachment),
//                     //     ImVec2(256, 256),
//                     //     ImVec2(0, 1),
//                     //     ImVec2(1, 0)
//                     // );

//                     ImGui::ColorEdit3("Ambient", &dirlight.ambient[0]);
//                     ImGui::ColorEdit3("Diffuse", &dirlight.diffuse[0]);
//                     ImGui::DragFloat("Intensity",  &dirlight.diffuse.w, 0.1f, 0.0f, 1.0f);
//                     ImGui::SliderFloat3("Direction", &dirlight.direction[0], -1.0f, 1.0f, "%.2f");

//                     if (ImGui::Button("Cancel"))
//                     {
//                         m_menu_action = "";
//                     }

//                 ImGui::EndChild();
//             }

//         if (ImGui::Button("Remove"))
//         {
//             ren.lightSystem().destroyDirlight(selected);
//             selected = 0;
//         }
//         ImGui::EndChild();
//         ImGui::EndChild();

//         if (ImGui::Button("Close"))
//         {
//             m_menu_action = "";
//         }

//         ImGui::EndChild();
//     }
// }


// void
// EditorUI_Module::ui_main_menu_bar()
// {
//     if (ImGui::BeginMainMenuBar())
//     {
//         if (ImGui::BeginMenu("File"))
//         {
//             if (ImGui::MenuItem("New", "CTRL + N"))
//             {

//             }

//             if (ImGui::MenuItem("Save As", "CTRL + S"))
//             {

//             }

//             if (ImGui::MenuItem("Load", "CTRL + L"))
//             {
                
//             }

//             ImGui::Separator();
//             ImGui::EndMenu();
//         }

//         if (ImGui::BeginMenu("Scene"))
//         {
//             ImGui::EndMenu();
//         }


//         if (ImGui::BeginMenu("Settings"))
//         {
//             if (ImGui::MenuItem("Graphics", ""))
//                 m_menu_action = "Settings_Graphics";

//             if (ImGui::MenuItem("Camera", ""))
//                 m_menu_action = "Settings_Camera";

//             if (ImGui::MenuItem("Skybox", ""))
//                 m_menu_action = "Settings_Skybox";

//             if (ImGui::MenuItem("Objects", ""))
//                 m_menu_action = "Settings_Objects";

//             if (ImGui::MenuItem("Dirlight", ""))
//                 m_menu_action = "Settings_Dirlight";

//             if (ImGui::MenuItem("Pointlight", ""))
//                 m_menu_action = "Settings_Pointlight";

//             ImGui::EndMenu();
//         }

//         float fps = ImGui::GetIO().Framerate;
//         ImGui::Text("Frame Time: %f", fps);

//         ImGui::EndMainMenuBar();
//     }
// }


// void
// EditorUI_Module::stage_A( idk::Engine &engine )
// {

// }


// void
// EditorUI_Module::stage_B( idk::Engine &engine )
// {
//     ImGui_ImplOpenGL3_NewFrame();
//     ImGui_ImplSDL2_NewFrame();
//     ImGui::NewFrame();

//     bool show = true;
//     ImGui::ShowDemoWindow(&show);
    
//     ui_main_menu_bar();

//     if (m_menu_action == "Settings_Graphics")
//     {
//         f_settings_graphics(engine);
//     }

//     if (m_menu_action == "Settings_Camera")
//     {
//         f_settings_camera(engine);
//     }

//     else if (m_menu_action == "Settings_Dirlight")
//     {
//         f_settings_dirlight(engine);
//     }

//     else if (m_menu_action == "Settings_Skybox")
//     {
//         f_settings_skybox(engine);
//     }

//     else if (m_menu_action == "Settings_Objects")
//     {
//         f_settings_objects(engine);
//     }

//     else if (m_menu_action == "Settings_Camera")
//     {
//         f_settings_camera(engine);
//     }

//     else if (m_menu_action == "Settings_Dirlight")
//     {
//         f_settings_dirlight(engine);
//     }

//     else if (m_menu_action == "Settings_Pointlight")
//     {
//         f_settings_pointlight(engine);
//     }

//     ImGui::Render();
//     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
// }



// void
// EditorUI_Module::stage_C( idk::Engine &engine )
// {

// }
