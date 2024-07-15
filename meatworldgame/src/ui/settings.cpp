#include "ui.hpp"


void createSettings( idk::EngineAPI &api, idkui2::LayoutManager *LM,
                     meatworld::GameData *gamedata )
{
    meatworld::GameUI       *gameui       = &(gamedata->gameui);
    meatworld::GameSettings *gamesettings = &(gamedata->settings);

    gameui->settings    = new idkui2::List("Settings", meatui::list_style);
    auto *audio_list    = new idkui2::List("Audio List",    meatui::list_style);
    auto *graphics_list = new idkui2::List("Graphics List", meatui::list_style);


    // Main
    // -----------------------------------------------------------------------------------------
    {
        gameui->settings->pushChildFront(new idkui2::Title("Settings", meatui::title_style));

        gameui->settings->pushChildFront(new idkui2::Button("Audio", meatui::button_style,
            [gameui, audio_list]()
            {
                gameui->root->m_children[1][1] = audio_list;
            }
        ));

        gameui->settings->pushChildFront(new idkui2::Button("Graphics", meatui::button_style,
            [gameui, graphics_list]()
            {
                gameui->root->m_children[1][1] = graphics_list;
            }
        ));

        gameui->settings->pushChildBack(new idkui2::Button("Return", meatui::button_style,
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
        audio_list->pushChildFront(new idkui2::Title("Audio", meatui::title_style));

        audio_list->pushChildFront(new idkui2::Button("audio0", meatui::button_style,
            [gameui, audio_list]()
            {
                std::cout << "audio0\n";
            }
        ));

        audio_list->pushChildFront(new idkui2::Button("audio1", meatui::button_style,
            [gameui, audio_list]()
            {
                std::cout << "audio1\n";
            }
        ));

        audio_list->pushChildFront(new idkui2::Button("audio2", meatui::button_style,
            [gameui, audio_list]()
            {
                std::cout << "audio2\n";
            }
        ));

        audio_list->pushChildBack(new idkui2::Button("Return", meatui::button_style,
            [gameui]()
            {
                gameui->root->m_children[1][1] = gameui->settings;
            }
        ));
    }
    // -----------------------------------------------------------------------------------------



    // Graphics
    // -----------------------------------------------------------------------------------------
    graphics_list->pushChildFront(new idkui2::Title("Graphics", meatui::title_style));

    graphics_list->pushChildFront(new idkui2::Button("Window Size", meatui::button_style,
        [gameui, graphics_list]()
        {
            
        }
    ));


    auto *split = new idkui2::Split("", meatui::splitwindow_style, 0.35f); 
    graphics_list->pushChildFront(split);

    split->setLeft(new idkui2::Label("Render Scale", meatui::label_style));

    split->setRight(
        new idkui2::Slider(
            "Render Scale",
            meatui::slider_style,
            gamesettings->res_scale,
            0.5f,
            2.0f,
            0.25f
        )
    );


    split = new idkui2::Split("", meatui::splitwindow_style, 0.35f); 
    graphics_list->pushChildFront(split);

    split->setLeft(new idkui2::Label("FOV", meatui::label_style));

    split->setRight(
        new idkui2::Slider(
            "FOV",
            meatui::slider_style,
            gamesettings->cam_fov,
            60.0f,
            100.0f,
            5.0f
        )
    );


    graphics_list->pushChildFront(new idkui2::Button("graphics1", meatui::button_style,
        [gameui, graphics_list]()
        {
            std::cout << "graphics1\n";
        }
    ));

    graphics_list->pushChildFront(new idkui2::Button("graphics2", meatui::button_style,
        [gameui, graphics_list]()
        {
            std::cout << "graphics2\n";
        }
    ));

    graphics_list->pushChildBack(new idkui2::Button("Return", meatui::button_style,
        [gameui]()
        {
            gameui->root->m_children[1][1] = gameui->settings;
        }
    ));
    // -----------------------------------------------------------------------------------------

}
