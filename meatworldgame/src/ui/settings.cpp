#include "ui.hpp"


void createSettings( idk::EngineAPI &api, idkui2::LayoutManager *LM,
                     meatworld::GameUI *gameui )
{
    idkui2::ElementStyle settingsstyle = {
        .invisible = true,
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 16.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };

    idkui2::ElementStyle buttonstyle = {
        .margin = glm::vec4(4.0f),
        .radius = 8.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };

    idkui2::ElementStyle sliderstyle = {
        .margin = glm::vec4(4.0f),
        .radius = 8.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f),
        .fg2    = glm::vec4(0.8f, 0.8f, 0.8f, 0.95f),
        .bg2    = glm::vec4(0.5f, 0.5f, 0.5f, 0.95f)
    };

    idkui2::ElementStyle textstyle = {
        .margin = glm::vec4(4.0f),
        .radius = 8.0f,
        .fg     = glm::vec4(0.8, 0.8f, 0.8f, 1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };

    idkui2::ElementStyle liststyle = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 52.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.1f, 0.1f, 0.1f, 0.95f)
    };



    gameui->settings    = new idkui2::Grid("Settings", settingsstyle, 1, 1);
    auto *settings_list = new idkui2::List("Settings List", liststyle);
    auto *audio_list    = new idkui2::List("Audio List",    liststyle);
    auto *graphics_list = new idkui2::List("Graphics List", liststyle);

    gameui->settings->setChild(0, 0, settings_list);


    // Main
    // -----------------------------------------------------------------------------------------
    settings_list->pushChildFront(new idkui2::Title("Settings", buttonstyle));

    settings_list->pushChildFront(
        new idkui2::Slider(
            "Slider", sliderstyle, gameui->slidertest, gameui->slidermin, gameui->slidermax, 0.25f
        )
    );

    settings_list->pushChildFront(new idkui2::TextInput("Text", sliderstyle, gameui->texttest));

    settings_list->pushChildFront(new idkui2::Button("Audio", buttonstyle,
        [gameui, audio_list]()
        {
            gameui->settings->setChild(0, 0, audio_list);
        }
    ));

    settings_list->pushChildFront(new idkui2::Button("Graphics", buttonstyle,
        [gameui, graphics_list]()
        {
            gameui->settings->setChild(0, 0, graphics_list);
        }
    ));

    settings_list->pushChildBack(new idkui2::Button("Return", buttonstyle,
        [gameui]()
        {
            gameui->settings->close();
        }
    ));
    // -----------------------------------------------------------------------------------------



    // Audio
    // -----------------------------------------------------------------------------------------
    audio_list->pushChildFront(new idkui2::Title("Audio", buttonstyle));

    // audio_list->pushChildFront(
    //     new idkui2::Slider(
    //         "Slider", sliderstyle, gameui->slidertest, gameui->slidermin, gameui->slidermax
    //     )
    // );

    audio_list->pushChildFront(new idkui2::Button("audio0", buttonstyle,
        [gameui, audio_list]()
        {
            std::cout << "audio0\n";
        }
    ));

    audio_list->pushChildFront(new idkui2::Button("audio1", buttonstyle,
        [gameui, audio_list]()
        {
            std::cout << "audio1\n";
        }
    ));

    audio_list->pushChildFront(new idkui2::Button("audio2", buttonstyle,
        [gameui, audio_list]()
        {
            std::cout << "audio2\n";
        }
    ));

    audio_list->pushChildBack(new idkui2::Button("Return", buttonstyle,
        [gameui, settings_list]()
        {
            gameui->settings->setChild(0, 0, settings_list);
        }
    ));
    // -----------------------------------------------------------------------------------------


    // Graphics
    // -----------------------------------------------------------------------------------------
    graphics_list->pushChildFront(new idkui2::Title("Graphics", buttonstyle));

    graphics_list->pushChildFront(new idkui2::Button("graphics0", buttonstyle,
        [gameui, graphics_list]()
        {
            std::cout << "graphics0\n";
        }
    ));

    graphics_list->pushChildFront(new idkui2::Button("graphics1", buttonstyle,
        [gameui, graphics_list]()
        {
            std::cout << "graphics1\n";
        }
    ));

    graphics_list->pushChildFront(new idkui2::Button("graphics2", buttonstyle,
        [gameui, graphics_list]()
        {
            std::cout << "graphics2\n";
        }
    ));

    graphics_list->pushChildBack(new idkui2::Button("Return", buttonstyle,
        [gameui, settings_list]()
        {
            gameui->settings->setChild(0, 0, settings_list);
        }
    ));
    // -----------------------------------------------------------------------------------------

}
