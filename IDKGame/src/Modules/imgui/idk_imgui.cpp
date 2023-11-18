#include "idk_imgui.h"

SDL_Window *m2_SDL_window;
SDL_GLContext m2_SDL_GL_Context;

void
ImGui_Module::init( idk::Engine &engine )
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(
        engine.rengine().SDLWindow(),
        engine.rengine().SDLGLContext()
    );

    ImGui_ImplOpenGL3_Init("#version 440");

    engine.eventManager().onSDLPollEvent(
        [](SDL_Event *event)
        {
            ImGui_ImplSDL2_ProcessEvent(event);
        }
    );

}


void
ImGui_Module::f_settings_graphics( idk::Engine &engine )
{
    idk::RenderEngine &ren = engine.rengine();

    if (ImGui::BeginChild("Settings_Graphics"))
    {
        ImGui::BeginChild("Grouping", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

        std::vector<std::string> names = ren.programNames();

        static int selected = 0;
        int n = 0;

        ImGuiStyle& style = ImGui::GetStyle();
        float child_w = 0.5 * ImGui::GetContentRegionAvail().x;
        ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);

        ImGui::BeginChild(child_id, ImVec2(child_w, 0), true, 0);
            for (std::string name: names)
            {
                idk::glShader &program = ren.getProgram(name);
                auto &definitions = program.getDefinitions();

                if (ImGui::Selectable(name.c_str(), selected == n))
                {
                    selected = n;
                }
                n += 1;
            }
        ImGui::EndChild();

        ImGui::SameLine();

        child_id = ImGui::GetID((void*)(intptr_t)1);
        ImGui::BeginChild(child_id, ImVec2(child_w, 0), true, 0);
            child_id = ImGui::GetID((void*)(intptr_t)2);
            ImGui::BeginChild("Defintions", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

                for (auto &[name, def]: ren.getProgram(names[selected]).getDefinitions())
                {
                    ImGui::InputText(name.c_str(), &def.value);
                }
            ImGui::EndChild();

            if (ImGui::Button("Recompile"))
            {
                ren.getProgram(names[selected]).compile();
            }
        ImGui::EndChild();
        ImGui::EndChild();

        if (ImGui::Button("Close"))
        {
            m_menu_action = "";
        }

        ImGui::EndChild();
    }
}


void
ImGui_Module::f_settings_camera( idk::Engine &engine )
{
    if (ImGui::BeginChild("Settings_Camera"))
    {
        idk::Camera &cam = engine.rengine().getCamera();

        static glm::vec3 offset(0.0f);
        ImGui::SliderFloat3("Offset", &offset[0], -2.0f, 2.0f, "%.3f");
        cam.offset(offset);


        ImGui::SliderFloat("Strength", &engine.rengine().m_abbr_str, 0.0f, 0.01f, "%.4f");
        ImGui::SliderFloat2("R",  &engine.rengine().m_r_abbr.x,  -1.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        if (ImGui::Button("RReset"))
            engine.rengine().m_r_abbr = glm::vec2(0.0f);

        ImGui::SliderFloat2("G",  &engine.rengine().m_g_abbr.x,  -1.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        if (ImGui::Button("GReset"))
            engine.rengine().m_g_abbr = glm::vec2(0.0f);

        ImGui::SliderFloat2("B",  &engine.rengine().m_b_abbr.x,  -1.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        if (ImGui::Button("BReset"))
            engine.rengine().m_b_abbr = glm::vec2(0.0f);


        static float bloom = 0.0001f;
        ImGui::SliderFloat("Bloom", &bloom, 0.0f, 0.005f, "%.5f");
        engine.rengine().setBloomIntensity(bloom);

        ImGui::SliderFloat("Exposure", &engine.rengine().m_exposure, 0.0f, 2.0f, "%.4f");
        ImGui::SliderFloat("Gamma", &engine.rengine().m_gamma, 0.0f, 4.0f, "%.4f");


        if (ImGui::Button("Cancel"))
        {
            m_menu_action = "";
        }

        ImGui::EndChild();
    }
}


void
ImGui_Module::f_settings_dirlight( idk::Engine &engine )
{
    idk::RenderEngine &ren = engine.rengine();


    if (ImGui::BeginChild("Settings_Dirlight"))
    {
        ImGui::BeginChild("REE", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

        static int selected = 0;
        int n = 0;

        ImGuiStyle& style = ImGui::GetStyle();
        float child_w = 0.5 * ImGui::GetContentRegionAvail().x;
        ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);

        ImGui::BeginChild(child_id, ImVec2(child_w, 0), true, 0);
            for (int i=0; i<ren.lightSystem().dirlights().size(); i++)
            {
                if (ImGui::Selectable(std::to_string(i).c_str(), selected == n))
                {
                    selected = n;
                }
                n += 1;
            }

            if (ImGui::Button("Add"))
            {
                ren.lightSystem().createLightsource(idk::lightsource::DIR);
            }
        ImGui::EndChild();

        ImGui::SameLine();

        child_id = ImGui::GetID((void*)(intptr_t)1);

        ImGui::BeginChild(child_id, ImVec2(child_w, 0), true, 0);

            child_id = ImGui::GetID((void*)(intptr_t)2);
    
            ImGui::BeginChild("REEEE", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
                auto &dirlight = ren.lightSystem().dirlights()[selected];

                ImGui::Image(
                    (void*)(intptr_t)(ren.lightSystem().shadowmaps()[selected].attachments[0]),
                    ImVec2(256, 256),
                    ImVec2(0, 1),
                    ImVec2(1, 0)
                );

                ImGui::ColorEdit3("Ambient", &dirlight.ambient[0]);
                ImGui::ColorEdit3("Diffuse", &dirlight.diffuse[0]);
                ImGui::SliderFloat3("Direction", &dirlight.direction[0], -1.0f, 1.0f, "%.2f");

                if (ImGui::Button("Cancel"))
                {
                    m_menu_action = "";
                }

            ImGui::EndChild();

        if (ImGui::Button("Remove"))
        {
            ren.lightSystem().destroyLightsource(idk::lightsource::DIR, selected);
            selected = 0;
        }
        ImGui::EndChild();
        ImGui::EndChild();

        if (ImGui::Button("Close"))
        {
            m_menu_action = "";
        }

        ImGui::EndChild();
    }
}


void
ImGui_Module::f_main_menu_bar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL + N"))
            {

            }

            if (ImGui::MenuItem("Save As", "CTRL + S"))
            {

            }

            if (ImGui::MenuItem("Load", "CTRL + L"))
            {
                
            }

            ImGui::Separator();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene"))
        {
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::MenuItem("Graphics", ""))
                m_menu_action = "Settings_Graphics";

            if (ImGui::MenuItem("Camera", ""))
                m_menu_action = "Settings_Camera";

            if (ImGui::MenuItem("Dirlight", ""))
                m_menu_action = "Settings_Dirlight";

            ImGui::EndMenu();
        }

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("Frame Time: %f", fps);

        ImGui::EndMainMenuBar();
    }
}


void
ImGui_Module::stage_A( idk::Engine &engine )
{

}


void
ImGui_Module::stage_B( idk::Engine &engine )
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    bool show = true;
    ImGui::ShowDemoWindow(&show);
    
    f_main_menu_bar();

    if (m_menu_action == "Settings_Graphics")
    {
        f_settings_graphics(engine);
    }

    if (m_menu_action == "Settings_Camera")
    {
        f_settings_camera(engine);
    }

    else if (m_menu_action == "Settings_Dirlight")
    {
        f_settings_dirlight(engine);
    }

    else if (m_menu_action == "Settings_Camera")
    {
        f_settings_camera(engine);
    }

    else if (m_menu_action == "Settings_Dirlight")
    {
        f_settings_dirlight(engine);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void
ImGui_Module::stage_C( idk::Engine &engine )
{

}
