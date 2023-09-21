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
    if (ImGui::BeginChild("Settings_Graphics"))
    {
        ImGui::Image(
            *(ImTextureID *)(void *)&engine.rengine().m_dirlight_depthmap_buffer.output_textures[0],
            {500, 500}, {0, 1}, {1, 0}
        );

        if(ImGui::Button("Cancel"))
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
    if (ImGui::BeginChild("Settings_Dirlight"))
    {

        auto &dirlight = engine.rengine().dirlights().get(0);


        ImGui::ColorEdit3("Ambient", &dirlight.ambient[0]);
        ImGui::ColorEdit3("Diffuse", &dirlight.diffuse[0]);
        ImGui::SliderFloat3("Direction", &dirlight.direction[0], -1.0f, 1.0f, "%.2f");



        if (ImGui::Button("Cancel"))
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

    // bool show = true;
    // ImGui::ShowDemoWindow(&show);
    
    f_main_menu_bar();

    if (m_menu_action == "Settings_Graphics")
    {
        f_settings_graphics(engine);
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
