#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"
#include "ComponentSystems/IDKcomponentsystems.h"
#include "Modules/idk_modules.h"



int ENTRY(int argc, const char **argv)
{
    idk::Engine engine("IDK Game", 1920, 1080, 1);
    idk::RenderEngine &ren = engine.rengine();

    const int TRANSFORM  = engine.registerCS<Transform_CS>("transform");
    const int MODEL      = engine.registerCS<Model_CS>("model");
    const int GRABBABLE  = engine.registerCS<Grabbable_CS>("grabbable");
    const int CHARACTER  = engine.registerCS<idkg::Character_CS>("character");
    const int CHARCONTROL= engine.registerCS<CharacterController_CS>("charactercontrol");
    const int CAMERA     = engine.registerCS<Camera_CS>("camera");

    auto &transCS = engine.getCS<Transform_CS>(TRANSFORM);
    auto &modelCS = engine.getCS<Model_CS>(MODEL);
    auto &grabCS  = engine.getCS<Grabbable_CS>(GRABBABLE);
    auto &charCS  = engine.getCS<CharacterController_CS>(CHARCONTROL);

    engine.registerModule<ImGui_Module>("imgui");


    engine.rengine().getCamera().elevation(2.0f);
    ren.getCamera().ylock(true);
    int cam2 = ren.createCamera();

    ren.loadSkybox("assets/textures/cubemaps/skybox2/");
    ren.loadSkybox("assets/textures/cubemaps/skybox4/");
    ren.loadSkybox("assets/textures/cubemaps/skybox5/");


    int player_obj = engine.createGameObject();
    int player_model = ren.modelManager().loadOBJ("assets/models/", "man.obj", "man.mtl");
    engine.giveComponents(player_obj, TRANSFORM, MODEL, CAMERA, CHARCONTROL);
    modelCS.useModel(player_obj, player_model, 0);
    charCS.controlMethod(player_obj, controlmethods::player);
    transCS.getTransform(player_obj).scale(glm::vec3(0.0f));
    transCS.translate(player_obj, glm::vec3(0.0f, -1.0f, 0.0f));


    int trash_obj = engine.createGameObject();
    int trash_model = ren.modelManager().loadOBJ("assets/models/", "ship.obj", "ship.mtl");
    engine.giveComponents(trash_obj, TRANSFORM, MODEL);
    modelCS.useModel(trash_obj, trash_model, 0);
    idk::Model &model = ren.modelManager().getModel(trash_model);
    // ren.modelManager().getMaterials().get(model.meshes[0].material_id).reflectance = idk::Material::IRON;
    // ren.modelManager().getMaterials().get(model.meshes[1].material_id).reflectance = idk::Material::IRON;
    transCS.translate(trash_obj, glm::vec3(0.0f, 0.0f, -2.0f));
    modelCS.setShadowcast(trash_obj, true);


    int hall_obj = engine.createGameObject();
    int hall_model = ren.modelManager().loadOBJ("assets/models/", "gun.obj", "gun.mtl");
    engine.giveComponents(hall_obj, TRANSFORM, MODEL);
    modelCS.useModel(hall_obj, hall_model, 0);
    transCS.translate(hall_obj, glm::vec3(20.0f, 0.0f, 0.0f));
    // modelCS.setShadowcast(hall_obj, true);


    // int soundobj = engine.createGameObject();
    // engine.giveComponent(soundobj, TRANSFORM);
    // int nier1 = engine.aengine().loadWav("assets/audio/nier2.wav");
    // int emitter = engine.aengine().createEmitter(nier1, transCS.getTransform(soundobj));
    // engine.aengine().playSound(emitter);
    // engine.aengine().listenerPosition(&transCS.getTransform(player_obj));

    int dir_id = ren.lightSystem().createDirlight(idk::LightFlag::SHADOWMAP);
    idk::Dirlight &light = ren.lightSystem().dirlights()[dir_id];
    light.ambient = glm::vec4(0.05f);
    light.diffuse.w = 1.0f;
    light.direction = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);

    // int point_id = ren.lightSystem().createPointlight();
    // idk::Pointlight &light = ren.lightSystem().pointlights()[point_id];
    // light.ambient     = glm::vec4(1.0f);
    // light.position    = glm::vec4(2.0f, 3.0f, 4.0f, 1.0f);
    // light.attenuation = glm::vec4(0.0f, 0.1f, 0.05f, 0.0f);

    while (engine.running())
    {
        engine.beginFrame();

        // if (engine.eventManager().keylog().keyDown(idk::Keycode::F))
        // {
        //     ren.useCamera(0);
        // }

        // else if (engine.eventManager().keylog().keyDown(idk::Keycode::G))
        // {
        //     ren.useCamera(1);
        // }


        // transCS.rotateY(sphere_obj, engine.deltaTime() * 0.1f); 

        // ren.drawPointlight(light);

        engine.endFrame();
    }


    return 0;
}
