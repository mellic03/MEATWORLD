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


    ren.modelManager().loadTextures("assets/textures/albedo/",    true);
    ren.modelManager().loadTextures("assets/textures/metallic/",  false);
    ren.modelManager().loadTextures("assets/textures/roughness/", false);
    ren.modelManager().loadTextures("assets/textures/ao/",        false);


    int player_obj = engine.createGameObject();
    int player_model = ren.modelManager().loadOBJ("assets/models/", "man.obj", "man.mtl");
    engine.giveComponents(player_obj, TRANSFORM, MODEL, CAMERA, CHARCONTROL);
    modelCS.useModel(player_obj, player_model, 0);
    charCS.controlMethod(player_obj, controlmethods::player);
    transCS.getTransform(player_obj).scale(glm::vec3(0.0f));
    transCS.translate(player_obj, glm::vec3(0.0f, -1.0f, 0.0f));


    int terrain_obj = engine.createGameObject();
    int terrain_model = ren.modelManager().loadOBJ("assets/models/", "hall.obj", "hall.mtl");
    engine.giveComponents(terrain_obj, TRANSFORM, MODEL);
    modelCS.useModel(terrain_obj, terrain_model, 0);


    int sphere_obj = engine.createGameObject();
    int sphere_model = ren.modelManager().loadOBJ("assets/models/", "sphere.obj", "sphere.mtl");
    engine.giveComponents(sphere_obj, TRANSFORM, MODEL);
    modelCS.useModel(sphere_obj, sphere_model, 0);
    transCS.translate(sphere_obj, glm::vec3(0.0f, 2.0f, 0.0f));

    // int soundobj = engine.createGameObject();
    // engine.giveComponent(soundobj, TRANSFORM);
    // int nier1 = engine.aengine().loadWav("assets/audio/nier2.wav");
    // int emitter = engine.aengine().createEmitter(nier1, transCS.getTransform(soundobj));
    // engine.aengine().playSound(emitter);
    // engine.aengine().listenerPosition(&transCS.getTransform(player_obj));


    int point_id = ren.lightSystem().createPointlight();
    idk::Pointlight &light = ren.lightSystem().pointlights()[point_id];
    light.position = glm::vec4(2.0f, 3.0f, 2.0f, 1.0f);

    while (engine.running())
    {
        engine.beginFrame();

        if (engine.eventManager().keylog().keyDown(idk::Keycode::F))
        {
            ren.useCamera(0);
        }

        else if (engine.eventManager().keylog().keyDown(idk::Keycode::G))
        {
            ren.useCamera(1);
        }


        // transCS.rotateY(sphere_obj, engine.deltaTime() * 1.0f); 

        ren.drawPointlight(light);

        engine.endFrame();
    }


    return 0;
}
