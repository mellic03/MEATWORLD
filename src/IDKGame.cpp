#include "IDKGame.hpp"

#include "ComponentSystems/componentsystems.hpp"
#include "Modules/idk_modules.hpp"

#include <iostream>


void
IDKGame::config()
{
    std::cout << "IDKGame config!\n";
}



void
IDKGame::registerModules( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();

    engine.registerModule<EditorUI_Module>("imgui");

    engine.registerCS<Transform_CS>("Transform");
    engine.registerCS<Model_CS>("Model");
    engine.registerCS<idkg::Camera_CS>("Camera");
    engine.registerCS<idkg::PlayerController_CS>("Control");
    engine.registerCS<idkg::Terrain_CS>("Terrain");
}



int player_obj, terrain_model;

void
IDKGame::setup( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    idk::ModelSystem  &MS  = ren.modelSystem();
    ren.getCamera().ylock(true);

    // Modules
    // -----------------------------------------------------------------------------------------
    const int TRANSFORM  = engine.getCS<Transform_CS>().ID();
    const int MODEL      = engine.getCS<Model_CS>().ID();
    const int CAMERA     = engine.getCS<idkg::Camera_CS>().ID();
    const int PCONTROL   = engine.getCS<idkg::PlayerController_CS>().ID();
    const int TERRAIN    = engine.getCS<idkg::Terrain_CS>().ID();

    auto &transCS   = engine.getCS<Transform_CS>();
    auto &modelCS   = engine.getCS<Model_CS>();
    auto &camCS     = engine.getCS<idkg::Camera_CS>();
    auto &terrainCS = engine.getCS<idkg::Terrain_CS>();
    // -----------------------------------------------------------------------------------------

    ren.loadSkybox("assets/cubemaps/skybox5/");
    ren.loadSkybox("assets/cubemaps/skybox1/");
    ren.loadSkybox("assets/cubemaps/skybox2/");
    ren.loadSkybox("assets/cubemaps/skybox3/");


    player_obj = engine.createGameObject();
    int player_model = ren.modelSystem().loadModel("assets/models/", "walk");
    engine.giveComponents(player_obj, TRANSFORM, MODEL, PCONTROL);
    modelCS.useModel(ren, player_obj, player_model);
    modelCS.setShadowcast(player_obj, true);
    transCS.translate(player_obj, glm::vec3(0.0f, 2.0f, -5.0f));


    int grass_obj     = engine.createGameObject();
    int grass_model   = ren.modelSystem().loadModel("assets/models/", "grass");
    engine.giveComponents(grass_obj, TRANSFORM, MODEL, TERRAIN);
    modelCS.useModel(ren, grass_obj, grass_model);

    int terrain_obj   = engine.createGameObject();
    GLuint heightmap  = MS.loadTexture("assets/heightmaps/terrain3.png", true);

    terrain_model = MS.loadTerrainHeightmap(heightmap);
    int material_A = MS.loadMaterial("assets/textures/", "Rock051_1K-PNG_Color.jpg", "", "");
    int material_B = MS.loadMaterial("assets/textures/", "grass2.jpg", "", "");
    MS.loadTerrainMaterials(terrain_model, material_A, material_B);

    engine.giveComponents(terrain_obj, TRANSFORM, MODEL, TERRAIN);
    terrainCS.useModel(terrain_obj, terrain_model);
    terrainCS.generateGrass(terrain_obj, terrain_model, heightmap, grass_model);



    int cart_obj = engine.createGameObject();
    int cart_model = ren.modelSystem().loadModel("assets/models/", "area");
    engine.giveComponents(cart_obj, TRANSFORM, MODEL);
    modelCS.useModel(ren, cart_obj, cart_model);
    modelCS.setShadowcast(cart_obj, true);


    int dir_id = ren.lightSystem().createDirlight(idk::LightFlag::SHADOWMAP);
    idk::Dirlight &light = ren.lightSystem().dirlights()[dir_id];
    light.ambient = glm::vec4(0.8f);
    light.diffuse.w = 1.0f;
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
}



void
IDKGame::mainloop( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    auto &transCS = engine.getCS<Transform_CS>();

    glm::vec3 pos = transCS.getTransform(player_obj).position();

    float height = ren.modelSystem().queryTerrainHeight(
        terrain_model, glm::mat4(1.0f), pos.x, pos.z
    );

    float &y = transCS.getTransform(player_obj).position_ptr()[1];

    y = height;
}

