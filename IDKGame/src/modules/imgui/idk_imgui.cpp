#include "idk_imgui.h"

SDL_Window *m2_SDL_window;
SDL_GLContext m2_SDL_gl_context;

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
    //     SDL_WINDOW_OPENGL
    // );
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


    // m2_SDL_gl_context = SDL_GL_CreateContext(m2_SDL_window);
    // SDL_GL_MakeCurrent(m2_SDL_window, m2_SDL_gl_context);
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
    ImGui_ImplOpenGL3_Init("#version 440");

    engine.eventManager().onSDLPollEvent(
        [](SDL_Event *event)
        {
            ImGui_ImplSDL2_ProcessEvent(event);
        }
    );

}


void
ImGui_Module::f_settings_graphics()
{

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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    // bool show = true;
    // ImGui::ShowDemoWindow(&show);

    f_main_menu_bar();


    if (m_menu_action == "Settings_Graphics")
    {
        if (ImGui::BeginChild("Settings_Graphics"))
        {
            ImGui::Text("WOWOWOWOW");
        
            if(ImGui::Button("Cancel"))
            {
                m_menu_action = "";
            }

            ImGui::EndChild();
        }
    }
}


void
ImGui_Module::stage_B( idk::Engine &engine )
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

