#include "ui.hpp"


void createSettings( idk::EngineAPI &api, idkui::LayoutManager *LM,
                     meatworld::GameData *gamedata )
{
    meatworld::GameUI       *gameui       = &(gamedata->gameui);
    meatworld::GameSettings *gamesettings = &(gamedata->settings);

    gameui->settings    = new idkui::List("Settings", meatui::list_style);
    auto *audio_list    = new idkui::List("Audio List",    meatui::list_style);
    auto *graphics_list = new idkui::List("Graphics List", meatui::list_style);


    // Main
    // -----------------------------------------------------------------------------------------
    {
        gameui->settings->pushChildFront(new idkui::Title("Settings", meatui::title_style));

        gameui->settings->pushChildFront(new idkui::Button("Audio", meatui::button_style,
            [gameui, audio_list]()
            {
                gameui->root->m_children[1][1] = audio_list;
            }
        ));

        gameui->settings->pushChildFront(new idkui::Button("Graphics", meatui::button_style,
            [gameui, graphics_list]()
            {
                gameui->root->m_children[1][1] = graphics_list;
            }
        ));

        gameui->settings->pushChildBack(new idkui::Button("Return", meatui::button_style,
            [gameui]()
            {
                gameui->root->m_children[1][1] = nullptr;
            }
        ));
    }
    // -----------------------------------------------------------------------------------------



    // Audio
    // -----------------------------------------------------------------------------------------
    {
        audio_list->pushChildFront(new idkui::Title("Audio", meatui::title_style));

        audio_list->pushChildFront(new idkui::Button("audio0", meatui::button_style,
            [gameui, audio_list]()
            {
                std::cout << "audio0\n";
            }
        ));

        audio_list->pushChildFront(new idkui::Button("audio1", meatui::button_style,
            [gameui, audio_list]()
            {
                std::cout << "audio1\n";
            }
        ));

        audio_list->pushChildFront(new idkui::Button("audio2", meatui::button_style,
            [gameui, audio_list]()
            {
                std::cout << "audio2\n";
            }
        ));

        audio_list->pushChildBack(new idkui::Button("Return", meatui::button_style,
            [gameui]()
            {
                gameui->root->m_children[1][1] = gameui->settings;
            }
        ));
    }
    // -----------------------------------------------------------------------------------------



    // Graphics
    // -----------------------------------------------------------------------------------------
    graphics_list->pushChildFront(new idkui::Title("Graphics", meatui::title_style));

    graphics_list->pushChildFront(new idkui::Button("1280x720", meatui::button_style,
        [&api, gameui, graphics_list]()
        {
            api.getRenderer().resize(1280, 720);
        }
    ));

    graphics_list->pushChildFront(new idkui::Button("1920x1080", meatui::button_style,
        [&api, gameui, graphics_list]()
        {
            api.getRenderer().resize(1920, 1080);
        }
    ));

    auto *split = new idkui::Split("", meatui::splitwindow_style, 0.35f); 
    graphics_list->pushChildFront(split);

    split->setLeft(new idkui::Label("Render Scale", meatui::label_style));

    split->setRight(
        new idkui::Slider(
            "Render Scale",
            meatui::slider_style,
            gamesettings->res_scale,
            0.5f,
            2.0f,
            0.25f
        )
    );


    split = new idkui::Split("", meatui::splitwindow_style, 0.35f); 
    graphics_list->pushChildFront(split);

    split->setLeft(new idkui::Label("FOV", meatui::label_style));

    split->setRight(
        new idkui::Slider(
            "FOV",
            meatui::slider_style,
            gamesettings->cam_fov,
            60.0f,
            100.0f,
            5.0f
        )
    );


    graphics_list->pushChildFront(new idkui::Button("graphics1", meatui::button_style,
        [gameui, graphics_list]()
        {
            std::cout << "graphics1\n";
        }
    ));

    graphics_list->pushChildFront(new idkui::Button("graphics2", meatui::button_style,
        [gameui, graphics_list]()
        {
            std::cout << "graphics2\n";
        }
    ));

    graphics_list->pushChildBack(new idkui::Button("Return", meatui::button_style,
        [gameui]()
        {
            gameui->root->m_children[1][1] = gameui->settings;
        }
    ));
    // -----------------------------------------------------------------------------------------

}
