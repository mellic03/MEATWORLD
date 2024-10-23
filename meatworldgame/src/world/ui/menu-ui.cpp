#include "menu-ui.hpp"
#include "ui-elements.hpp"
#include "../world.hpp"

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/idk_render_settings.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKIO/IDKIO.hpp>


idkui::Element*
meatui::ingame_menu( idk::EngineAPI &api, meat::World &world )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &UI     = world.ui;
    auto &EM     = UI.EM;

    auto btn = [&EM]( std::string label, const std::function<void()> &callback )
    {
        return new Button(label, callback);
    };

    auto *menu = new meatui::Menu("Paused", {
        btn("Continue",    [&UI, &EM](){ EM.emit("menu-close"); EM.emit("menu-open", UI.ingame_ui); }),
        btn("Multiplayer", [&UI, &EM](){ EM.emit("menu-open", UI.multiplayer); }),
        btn("Settings",    [&UI, &EM](){ EM.emit("menu-open", UI.settings); }),
        btn("Exit",        [&engine] (){ engine.shutdown(); })
    });

    return menu;

    // Main
    // -----------------------------------------------------------------------------------------
    // menu->pushChildFront(new idkui::Title("Paused", meatui::title_style));

    // menu->pushChildFront(new idkui::Button("Continue", meatui::button_style,
    //     []()
    //     {
    //         idkio::mouseCapture(true);
    //     }
    // ));

    // menu->pushChildFront(new idkui::Button("Multiplayer", meatui::button_style,
    //     []()
    //     {
    //         // gameui->ingame_menu->m_children[1][1] = gameui->multiplayer;
    //     }
    // ));

    // menu->pushChildFront(new idkui::Button("Settings", meatui::button_style,
    //     [&world]()
    //     {
    //         world.ui.root->m_children[1][1] = world.ui.settings;
    //         // gameui->ingame_menu->m_children[1][1] = gameui->settings;
    //     }
    // ));

    // menu->pushChildFront(new idkui::Button("Exit", meatui::button_style,
    //     [&engine]()
    //     {
    //         engine.shutdown();
    //         // idk::ECS2::load("scenes/mainmenu");
    //     }
    // ));
    // -----------------------------------------------------------------------------------------

}



idkui::Element*
meatui::settings_menu( idk::EngineAPI &api, meat::World &world )
{
    auto &ren = api.getRenderer();
    auto &UI  = world.ui;
    auto &EM  = UI.EM;


    auto btn = [&EM]( std::string label, const std::function<void()> &callback )
    {
        return new Button(label, callback);
    };

    auto btntaa1 = [&ren, &EM]()
    {
        return new Button("TAA 0x", [&ren, &EM](){
            auto ree = ren.getRenderSettings();
            ree.taa.factor = 1;
            ren.applyRenderSettings(ree);
        });
    };


    auto btntaa2 = [&ren, &EM]()
    {
        return new Button("TAA 2x", [&ren, &EM](){
            auto ree = ren.getRenderSettings();
            ree.taa.factor = 2;
            ren.applyRenderSettings(ree);
        });
    };


    auto btnssr2 = [&ren, &EM]()
    {
        return new Button("SSR off", [&ren, &EM](){
            auto ree = ren.getRenderSettings();
            ree.ssr.enabled = false;
            ren.applyRenderSettings(ree);
        });
    };

    auto btnssr1 = [&ren, &EM]()
    {
        return new Button("SSR on", [&ren, &EM](){
            auto ree = ren.getRenderSettings();
            ree.ssr.enabled = true;
            ren.applyRenderSettings(ree);
        });
    };


    auto *audio = new meatui::Menu("Audio", {
        btn("Test Audio", [](){}),
        btn("Return",   [&EM] (){ EM.emit("menu-close"); })
    });

    auto *graphics = new meatui::Menu("Graphics", {
        btn("1/1 Scale", [&ren](){ ren.setRenderScale(1.0f); }),
        btn("1/2 Scale", [&ren](){ ren.setRenderScale(0.5f); }),

        new idkui::Text("Resolution", meatui::button_style),
        btn("1280x720 ", [&ren](){ ren.resize(1280, 720);  }),
        btn("1920x1080", [&ren](){ ren.resize(1920, 1080); }),
        btn("2560x1440", [&ren](){ ren.resize(2560, 1440); }),

        btntaa1(),
        btntaa2(),
        btnssr1(),
        btnssr2(),
        btn("Reload shaders", [&ren](){ ren.recompileShaders(); }),
        btn("Return",         [&EM] (){ EM.emit("menu-close"); })
    });

    auto *system = new meatui::Menu("System", {
        btn("Reload modules", [&api](){ api.getEngine().reloadModules(); }),
        btn("Return",         [&EM] (){ EM.emit("menu-close"); })
    });

    auto *menu = new meatui::Menu("Settings", {
        btn("Audio",    [&EM, audio]   (){ EM.emit("menu-open", audio); }),
        btn("Graphics", [&EM, graphics](){ EM.emit("menu-open", graphics); }),
        btn("System",   [&EM, system]  (){ EM.emit("menu-open", system); }),
        btn("Return",   [&EM]          (){ EM.emit("menu-close"); })
    });


    return menu;
}

