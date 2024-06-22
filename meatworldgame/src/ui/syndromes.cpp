#include "ui.hpp"


void
createSyndromes( idk::EngineAPI &api, idkui2::LayoutManager *LM, meatworld::GameUI *gameui )
{
    idkui2::ElementStyle settingsstyle = {
        .invisible = true,
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 16.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
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
        .radius = 16.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f)
    };

    idkui2::ElementStyle syndromestyle = {
        .margin  = glm::vec4(9.0f),
        .padding = glm::vec4(0.0f, 0.0f, 0.0f, 64.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = glm::vec4(0.25f, 0.25f, 0.25f, 0.95f),
        .uniform_size = true
    };


    gameui->syndromes = new idkui2::Grid("Syndromes", syndromestyle, 5, 5);
    gameui->syndromes->close();

    for (int i=0; i<5; i++)
    {
        for (int j=0; j<5; j++)
        {
            gameui->syndromes->setChild(i, j, new idkui2::Grid("#", buttonstyle, 1, 1));
        }
    }

    gameui->root->setChild(0, 2, gameui->syndromes);

}

