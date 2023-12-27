#include <iostream>
#include "IDKengine/IDKengine.hpp"
#include "ComponentSystems/componentsystems.hpp"
#include "Modules/idk_modules.hpp"


int main( int argc, const char **argv )
{
    idk::Engine engine("IDK Game", 1920, 1080, 1);
    idk::RenderEngine &ren = engine.rengine();
    ren.getCamera().ylock(true);

    engine.registerModule<EditorUI_Module>("imgui");

    const int TRANSFORM  = engine.registerCS<Transform_CS>("Transform");
    const int MODEL      = engine.registerCS<Model_CS>("Model");
    const int CAMERA     = engine.registerCS<idkg::Camera_CS>("Camera");
    // const int PCONTROL   = engine.registerCS<idkg::PlayerController_CS>("Control");
    const int TERRAIN    = engine.registerCS<idkg::Terrain_CS>("Terrain");

    auto &transCS   = engine.getCS<Transform_CS>();
    auto &modelCS   = engine.getCS<Model_CS>();
    auto &camCS     = engine.getCS<idkg::Camera_CS>();
    auto &terrainCS = engine.getCS<idkg::Terrain_CS>();

    engine.initModules();


    ren.loadSkybox("assets/cubemaps/skybox5/");
    ren.loadSkybox("assets/cubemaps/skybox1/");
    ren.loadSkybox("assets/cubemaps/skybox2/");
    ren.loadSkybox("assets/cubemaps/skybox3/");


    int player_obj = engine.createGameObject();
    int player_model = ren.modelSystem().loadModel("assets/models/", "walk");
    engine.giveComponents(player_obj, TRANSFORM, MODEL);
    modelCS.useModel(player_obj, player_model);
    modelCS.setShadowcast(player_obj, true);
    transCS.translate(player_obj, glm::vec3(0.0f, 2.0f, -5.0f));


    int grass_obj     = engine.createGameObject();
    int grass_model   = ren.modelSystem().loadModel("assets/models/", "grass");
    engine.giveComponents(grass_obj, TRANSFORM, MODEL, TERRAIN);
    modelCS.useModel(grass_obj, grass_model);

    int terrain_obj   = engine.createGameObject();
    int terrain_model = ren.modelSystem().loadModel("assets/models/", "terrain");
    
    engine.giveComponents(terrain_obj, TRANSFORM, MODEL, TERRAIN);
    terrainCS.useModel(terrain_obj, terrain_model);
    modelCS.setShadowcast(terrain_obj, true);
    terrainCS.generateGrass(terrain_obj, terrain_model, grass_model);


    int cart_obj = engine.createGameObject();
    int cart_model = ren.modelSystem().loadModel("assets/models/", "area");
    engine.giveComponents(cart_obj, TRANSFORM, MODEL);
    modelCS.useModel(cart_obj, cart_model);
    modelCS.setShadowcast(cart_obj, true);


    int dir_id = ren.lightSystem().createDirlight(idk::LightFlag::SHADOWMAP);
    idk::Dirlight &light = ren.lightSystem().dirlights()[dir_id];
    light.ambient = glm::vec4(0.8f);
    light.diffuse.w = 1.0f;
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);


    while (engine.running())
    {
        engine.beginFrame();
    
        // Do stuff

        engine.endFrame();
    }


    return 0;
}
