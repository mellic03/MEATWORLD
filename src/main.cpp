#include "IDKGame.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKEvents/IDKEvents.hpp>
#include <libidk/idk_octree.hpp>

#include "./state.hpp"

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


void
IDKGame::setup( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ecs      = api.getECS();
    auto &eventsys = api.getEventSys();
    auto &ren      = api.getRenderer();

    ecs.getSystem<idk::ScriptSys>().loadScripts("assets/scripts/");
    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox1/"));

    api.getEventSys().onDropFile(".idksc", [&api](const char *path)
    {
        api.getECS().readFile(path);
    });


    int dir_id = ren.lightSystem().createDirlight(idk::LightFlag::SHADOWMAP);
    idk::Dirlight &light = ren.lightSystem().dirlights()[dir_id];
    light.ambient = glm::vec4(0.0f);
    light.diffuse = glm::vec4(1.0f);
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
}



void
IDKGame::mainloop( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ecs      = api.getECS();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

    // idkg::update_player(api);

    // int player = ecs.getGameObjectByName("Player");
    // int moon   = ecs.getGameObjectByName("Moon");
    // int rect   = ecs.getGameObjectByName("bounding-rect");

    // if (player == -1)
    // {
    //     return;
    // }

    // static bool first = true;
    // if (first)
    // {
    //     first = false;
    // }

    // for (idkg::Civillian &civ: m_civillians)
    // {
    //     civ.update(api, player);
    // }



    // int planet = ecs.getGameObjectByName("Planet");
    // ecs.getComponent<idk::ModelCmp>(planet).visible = false;


    // static int program = ren.createProgram("gpass-planet", "assets/shaders/", "gpass-planet.vs", "gpass-planet.fs");
    // static int RQ = ren.createRenderQueue("gpass-planet");

    // int model = ecs.getComponent<idk::ModelCmp>(planet).model_id;

    // static int mat = ren.modelAllocator().createMaterial(
    //     "assets/heightmaps/terrain.png",
    //     "assets/heightmaps/terrain2.png",
    //     "assets/heightmaps/terrain3.png"
    // );

    // ren.modelAllocator().addUserMaterial(model, mat, 0);
    // ren.modelAllocator().addUserMaterial(model, mat, 1);

    // ren.drawModelRQ(
    //     RQ,
    //     model,
    //     glm::translate(glm::mat4(1.0f), glm::vec3(303.0f, 43.0f, -1986.0f))
    // );


}

