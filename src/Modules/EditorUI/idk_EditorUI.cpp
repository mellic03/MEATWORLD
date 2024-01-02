#include "idk_EditorUI.hpp"

#include "tabs/EditorUI-tabs.hpp"


void
EditorUI_Module::init( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingTransparentPayload = true;

    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(
        ren.SDLWindow(),
        ren.SDLGLContext()
    );

    ImGui_ImplOpenGL3_Init("#version 440");

    engine.eventManager().onSDLPollEvent(
        [](SDL_Event *event)
        {
            ImGui_ImplSDL2_ProcessEvent(event);
        }
    );


    for (size_t i=0; i<NUM_FONTS; i++)
    {
        int font_size = FONT_SIZES[i];

        std::string path = "assets/fonts/Ubuntu/"
                         + std::to_string(font_size)
                         + "/Ubuntu-Medium.ttf";

        m_fonts[i] = io.Fonts->AddFontFromFileTTF(path.c_str(), float(font_size));


        // Icons
        // -------------------------------------------------------------------------------------
        ImFontConfig config;
        config.MergeMode = true;
        // config.PixelSnapH = true;
        static const ImWchar icon_ranges[] = { IDK_FONT_ICON_MIN, IDK_FONT_ICON_MAX, 0 };

        float icon_font_size = float(font_size) * IDK_FONT_ICON_SCALE_FACTOR;
        io.Fonts->AddFontFromFileTTF(IDK_FONT_ICON_FILEPATH, icon_font_size, &config, icon_ranges);

        // icon_font_size = float(font_size) * IDK_FONT_ICON_SCALE_FACTOR2;
        io.Fonts->AddFontFromFileTTF(IDK_FONT_ICON_FILEPATH2, icon_font_size, &config, icon_ranges);
        // -------------------------------------------------------------------------------------
    }

    io.FontDefault = m_fonts[1];

}



void
EditorUI_Module::stage_B( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (m_show_ImGui_demo)
    {
        ImGui::ShowDemoWindow(&m_show_ImGui_demo);
    }

    this->_menubar(api);
    this->_tab(api);


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

