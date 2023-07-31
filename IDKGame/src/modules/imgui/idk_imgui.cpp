#include "idk_imgui.h"

SDL_Window *m2_SDL_window;
SDL_GLContext m2_SDL_GL_Context;

void
ImGui_Module::init( idk::Engine &engine )
{
    // if (SDL_Init(SDL_INIT_VIDEO) < 0)
    // {
    //     std::cout << "Error creating window\n";
    //     exit(1);
    // }

    // m2_SDL_window = SDL_CreateWindow(
    //     "test",
    //     SDL_WINDOWPOS_CENTERED,
    //     SDL_WINDOWPOS_CENTERED,
    //     500,
    //     500,
    //     SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    // );
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    // m2_SDL_GL_Context = SDL_GL_CreateContext(m2_SDL_window);
    // SDL_GL_MakeCurrent(m2_SDL_window, m2_SDL_GL_Context);
    // SDL_SetRelativeMouseMode(SDL_FALSE);

    // if (glewInit() != GLEW_OK)
    // {
    //     std::cout << "Error initializing glew\n";
    //     exit(1);
    // }


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

    // ImGui_ImplSDL2_InitForOpenGL(
    //     m2_SDL_window,
    //     m2_SDL_GL_Context
    // );

    ImGui_ImplOpenGL3_Init("#version 440");

    engine.eventManager().onSDLPollEvent(
        [](SDL_Event *event)
        {
            ImGui_ImplSDL2_ProcessEvent(event);
        }
    );

    // SDL_GL_MakeCurrent(engine.rengine().SDLWindow(), engine.rengine().SDLGLContext());
}


void
ImGui_Module::f_settings_graphics( idk::Engine &engine )
{
    if (ImGui::BeginChild("Settings_Graphics"))
    {
        ImGui::Text("WOWOWOWOW");
    

        // for (int i=0; i<4; i++)
        // {

        //     ImGui::Image(
        //         *(ImTextureID *)(void *)&engine.rengine().m_dirlight_depthmap_buffer.output_textures[i],
        //         {500, 500}, {0, 1}, {1, 0}
        //     );
        // }


        if(ImGui::Button("Cancel"))
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
    // SDL_GL_MakeCurrent(m2_SDL_window, m2_SDL_GL_Context);
    // idk::gl::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // idk::gl::clear(GL_COLOR_BUFFER_BIT);

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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // SDL_GL_SwapWindow(m2_SDL_window);


    // SDL_GL_MakeCurrent(engine.rengine().SDLWindow(), engine.rengine().SDLGLContext());
}



void
ImGui_Module::stage_C( idk::Engine &engine )
{

}
