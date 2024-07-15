#include "ui.hpp"

#include <IDKEvents/IDKEvents.hpp>


void
createSyndromes( idk::EngineAPI &api, idkui2::LayoutManager *LM, meatworld::GameData *gamedata )
{
    meatworld::GameUI *gameui = &(gamedata->gameui);

    idkui2::ElementStyle settingsstyle = {
        .invisible = true,
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 16.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f)
    };

    idkui2::ElementStyle liststyle = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 64.0f),
        .radius  = 16.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.1f, 0.1f, 0.1f, 0.95f)
    };

    idkui2::ElementStyle buttonstyle = {
        .margin = glm::vec4(4.0f),
        .radius = 4.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.8f, 0.8f, 0.8f, 0.95f)
    };

    idkui2::ElementStyle syndromestyle = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f),
        .uniform_size = true
    };

    idkui2::ElementStyle title_style = {
        .margin = glm::vec4(4.0f),
        .radius = 4.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(1.0f, 1.0f, 0.0f, 0.95f)
    };


    idkui2::ElementStyle styleA = {
        .margin  = glm::vec4(8.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(1.0f, 0.0f, 0.0f, 0.95f),
        .uniform_size = true
    };

    idkui2::ElementStyle styleB = {
        .margin  = glm::vec4(8.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 0.5f),
        .uniform_size = true
    };

    idkui2::ElementStyle styleC = {
        .margin  = glm::vec4(8.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.0f, 0.0f, 1.0f, 0.95f),
        .uniform_size = true
    };


    gameui->syndromes = new idkui2::Grid("", settingsstyle, 3, 1);
    gameui->syndromes->setRowProportions({0.25f, 0.5f, 0.25f});
    gameui->syndromes->close();

    auto *list_top = new idkui2::List("", styleA);
    auto *grid_mid = new idkui2::Grid("", styleB, 5, 5);
    auto *list_bot = new idkui2::List("", styleC);

    gameui->syndromes->setChild(0, 0, list_top);
    gameui->syndromes->setChild(1, 0, grid_mid);
    gameui->syndromes->setChild(2, 0, list_bot);


    list_top->pushChildBack(new idkui2::Title("Syndromes", meatui::title_style));

    for (int i=0; i<5; i++)
    {
        for (int j=0; j<5; j++)
        {
            grid_mid->setChild(i, j, new idkui2::Button("", buttonstyle, [](){}));
        }
    }

    api.getEventSys().onKeyTapped(idk::Keycode::TAB,
        [gameui]()
        {
            gameui->root->setChild(1, 1, gameui->syndromes);
            gameui->syndromes->m_children[1][0]->m_visible = true;
            gameui->syndromes->open();
        }
    );

}

