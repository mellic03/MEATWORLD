#include "IDKGame.hpp"
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include "ComponentSystems/componentsystems.hpp"
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

    // engine.registerModule<EditorUI_Module>("imgui");

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
    const int MODEL      = engine.getCS<idk::Model_CS>().ID();
    const int CAMERA     = engine.getCS<idk::Camera_CS>().ID();

    const int PCONTROL   = engine.getCS<idkg::PlayerController_CS>().ID();
    const int TERRAIN    = engine.getCS<idkg::Terrain_CS>().ID();


    auto &transCS   = engine.getCS<idk::Transform_CS>();
    auto &modelCS   = engine.getCS<idk::Model_CS>();
    auto &camCS     = engine.getCS<idk::Camera_CS>();

    auto &terrainCS = engine.getCS<idkg::Terrain_CS>();
    // -----------------------------------------------------------------------------------------

    // ren.loadSkybox("assets/cubemaps/skybox5/");
    // ren.loadSkybox("assets/cubemaps/skybox1/");
    ren.loadSkybox("assets/cubemaps/skybox2/");
    // ren.loadSkybox("assets/cubemaps/skybox3/");


    player_obj = engine.createGameObject("player");
    int player_model = ren.modelSystem().loadModel("assets/models/", "walk");
    engine.giveComponents(player_obj, MODEL, PCONTROL);
    modelCS.useModel(ren, player_obj, player_model);
    modelCS.setShadowcast(player_obj, true);
    transCS.translate(player_obj, glm::vec3(0.0f, 2.0f, -5.0f));


    int grass_obj     = engine.createGameObject("grass");
    int grass_model   = ren.modelSystem().loadModel("assets/models/", "grass");
    engine.giveComponents(grass_obj, MODEL, TERRAIN);
    modelCS.useModel(ren, grass_obj, grass_model);

    int terrain_obj   = engine.createGameObject("terrain");
    GLuint heightmap  = MS.loadTexture("assets/heightmaps/terrain3.png", true);

    terrain_model = MS.loadTerrainHeightmap(heightmap);
    int material_A = MS.loadMaterial("assets/textures/", "Rock051_1K-PNG_Color.jpg", "", "");
    int material_B = MS.loadMaterial("assets/textures/", "grass2.jpg", "", "");
    MS.loadTerrainMaterials(terrain_model, material_A, material_B);

    engine.giveComponents(terrain_obj, MODEL, TERRAIN);
    terrainCS.useModel(terrain_obj, terrain_model);
    terrainCS.generateGrass(terrain_obj, terrain_model, heightmap, grass_model);


    int area_obj = engine.createGameObject("area");
    int area_model = ren.modelSystem().loadModel("assets/models/", "area");
    engine.giveComponents(area_obj, MODEL);
    modelCS.useModel(ren, area_obj, area_model);
    modelCS.setShadowcast(area_obj, true);


    // int cart_obj = engine.createGameObject("cart 1");
    // int cart_model = ren.modelSystem().loadModel("assets/models/", "cart");
    // engine.giveComponents(cart_obj, MODEL);
    // modelCS.useModel(ren, cart_obj, cart_model);
    // modelCS.setShadowcast(cart_obj, true);

    // cart_obj = engine.createGameObject("cart 2");
    // engine.giveComponents(cart_obj, MODEL);
    // modelCS.useModel(ren, cart_obj, cart_model);
    // modelCS.setShadowcast(cart_obj, true);


    int dir_id = ren.lightSystem().createDirlight(idk::LightFlag::SHADOWMAP);
    idk::Dirlight &light = ren.lightSystem().dirlights()[dir_id];
    light.ambient = glm::vec4(0.8f);
    light.diffuse.w = 1.0f;
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
}


#include <filesystem>

void
IDKGame::mainloop( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();


    if (engine.eventManager().fileDropped())
    {
        std::string filepath = engine.eventManager().fileDroppedPath();
                    filepath = std::filesystem::relative(filepath);

        size_t i = filepath.length() - 1;
        for (i=filepath.length()-1; i>=0; i--)
        {
            if (filepath[i] == '/')
            {
                break;
            }
        }

        std::string name = filepath.substr(i+1);
        
        int obj_id   = engine.createGameObject(name);
        int model_id = ren.modelSystem().loadModel(filepath + "/", name);

        engine.giveComponent<idk::Model_CS>(obj_id);
        engine.getCS<idk::Model_CS>().useModel(ren, obj_id, model_id);
    }


    auto &transCS = engine.getCS<idk::Transform_CS>();

    glm::vec3 &pos = transCS.getPosition(player_obj);

    float height = ren.modelSystem().queryTerrainHeight(
        terrain_model, glm::mat4(1.0f), pos.x, pos.z
    );

    pos.y = height;
}

