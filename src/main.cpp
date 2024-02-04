#include "IDKGame.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKEvents/IDKEvents.hpp>

#include "ComponentSystems/componentsystems.hpp"
#include <iostream>


void
IDKGame::config()
{

}


void
IDKGame::registerModules( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();

    // engine.registerCS<idkg::PlayerController_CS>("Control");
    // engine.registerCS<idkg::Terrain_CS>("Terrain");
}


static void ecs_GiveComponents( idecs::ECS &ecs, int obj_id, int model=-1 )
{
    ecs.giveComponent<idk::IconCmp>(obj_id);
    ecs.giveComponent<idk::TransformCmp>(obj_id);

    if (model != -1)
    {
        idk::ModelSys::assignModel(obj_id, model);
        // ecs.giveComponent<idk::ModelCmp>(obj_id);
        // auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);
        // cmp.obj_id = obj_id;
        // cmp.model_id = model;
    }
}


int player_obj, cam_obj, terrain_model;

void
IDKGame::setup( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ecs      = api.getECS();
    auto &eventsys = api.getEventSys();
    auto &ren      = api.getRenderer();
    auto &MS       = ren.modelSystem();

    ecs.getSystem<idk::ScriptSys>().loadScripts("assets/scripts/");
    ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox3/"));


    // player_obj = ecs.createGameObject("player");
    // ecs_GiveComponents(ecs, player_obj);

    // cam_obj = ecs.createGameObject("camera");
    // ecs_GiveComponents(ecs, cam_obj);
    // ecs.giveComponent<idk::CameraCmp>(cam_obj);
    // ecs.getComponent<idk::CameraCmp>(cam_obj).obj_id = cam_obj;
    // ecs.getComponent<idk::CameraCmp>(cam_obj).cam_id = 0;

    // int area_obj = ecs.createGameObject("area");
    // int area_model = ren.modelSystem().loadModel("assets/models/", "area");
    // ecs_GiveComponents(ecs, area_obj, area_model);

    // int cart_obj = ecs.createGameObject("cart 1");
    // int cart_model = ren.modelSystem().loadModel("assets/models/", "cart");
    // ecs_GiveComponents(ecs, cart_obj, cart_model);

    // int city_obj = ecs.createGameObject("city");
    // int city_model = ren.modelSystem().loadModel("assets/models/", "city");
    // ecs_GiveComponents(ecs, city_obj, city_model);

    // int cube_obj = ecs.createGameObject("cube");
    // int cube_model = MS.loadModel("assets/models/", "unit-cube");
    // ecs_GiveComponents(ecs, cube_obj, cube_model);


    // MS.loadModel("assets/models/", "pipe-cross");
    // MS.loadModel("assets/models/", "pipe-elbow");
    // MS.loadModel("assets/models/", "a-wall");
    // MS.loadModel("assets/models/", "a-wall-corner");
    // MS.loadModel("assets/models/", "a-wall-door");
    // MS.loadModel("assets/models/", "a-ground");
    // MS.loadModel("assets/models/", "a-ramp-single");
    // MS.loadModel("assets/models/", "a-ramp-double");

    int dir_id = ren.lightSystem().createDirlight(idk::LightFlag::SHADOWMAP);
    idk::Dirlight &light = ren.lightSystem().dirlights()[dir_id];
    light.ambient = glm::vec4(0.1f);
    light.diffuse = glm::vec4(1.0f);
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
}



void
IDKGame::mainloop( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

}

