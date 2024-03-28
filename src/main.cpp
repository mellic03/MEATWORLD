#include "IDKGame.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include <iostream>


void
IDKGame::config()
{

}


void
IDKGame::registerModules( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
}


int model;


void
IDKGame::setup( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &eventsys = api.getEventSys();
    auto &ren      = api.getRenderer();

    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox1/"));
    model = ren.loadModel("assets/models/cave.idkvi");


    int light_id = ren.createDirlight();
    IDK_Dirlight &light = ren.getDirlight(light_id);

    light.ambient   = glm::vec4(0.0f);
    light.diffuse   = glm::vec4(1.0f);
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);

}


void
IDKGame::mainloop( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

    ren.drawModel(model, glm::mat4(1.0f));

}

