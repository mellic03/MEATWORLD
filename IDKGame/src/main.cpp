#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"
#include "ComponentSystems/IDKcomponentsystems.h"
#include "modules/idk_modules.h"




int ENTRY(int argc, const char **argv)
{
    idk::Engine engine("IDK Game", 1920, 1080, 1);
    idk::RenderEngine &ren = engine.rengine();

    const int TRANSFORM  = engine.registerCS<Transform_CS>("transform");
    const int MODEL      = engine.registerCS<Model_CS>("model");
    const int GRABBABLE  = engine.registerCS<Grabbable_CS>("grabbable");
    const int POINTLIGHT = engine.registerCS<PointLight_CS>("pointlight");
    const int CHARCONTROL= engine.registerCS<CharacterController_CS>("charactercontrol");
    const int SPOTLIGHT  = engine.registerCS<SpotLight_CS>("spotlight");
    const int CAMERA     = engine.registerCS<Camera_CS>("camera");


    auto &transCS = engine.getCS<Transform_CS>(TRANSFORM);
    auto &modelCS = engine.getCS<Model_CS>(MODEL);
    auto &pointCS = engine.getCS<PointLight_CS>(POINTLIGHT);
    auto &spotCS  = engine.getCS<SpotLight_CS>(SPOTLIGHT);
    auto &grabCS  = engine.getCS<Grabbable_CS>(GRABBABLE);
    auto &charCS  = engine.getCS<CharacterController_CS>(CHARCONTROL);


    engine.registerModule<ImGui_Module>("imgui");
    // engine.registerModule<idk_Voxel>("voxel");


    ren.modelManager().loadIDKtexpak("assets/textures/diffuse.texpak",  true);
    ren.modelManager().loadIDKtexpak("assets/textures/specular.texpak", false);
    ren.modelManager().loadIDKtexpak("assets/textures/reflection.texpak", false);
    ren.getCamera().ylock(true);


    // int terrain2_obj = engine.createGameObject();
    // int terrain2_model = ren.modelManager().loadOBJ("assets/models/", "man.obj", "man.mtl");
    // engine.giveComponents(terrain2_obj, TRANSFORM, MODEL);
    // transCS.translate(terrain2_obj, glm::vec3(0.0f, 15.0f, 2.0f));
    // modelCS.useModel(terrain2_obj, terrain2_model, default_geometrypass);


    int player_obj = engine.createGameObject();
    engine.giveComponents(player_obj, TRANSFORM, CAMERA, CHARCONTROL);
    charCS.controlMethod(player_obj, controlmethods::player);

    engine.rengine().getCamera().translate(glm::vec3(0.0f, 5.0f, 5.0f));
    engine.rengine().getCamera().elevation(2.0f);


    int platform_obj = engine.createGameObject();
    int platform_model = ren.modelManager().loadOBJ("assets/models/", "hall.obj", "hall.mtl");
    engine.giveComponents(platform_obj, TRANSFORM, MODEL);
    modelCS.useModel(platform_obj, platform_model, 0);


    int angel_obj = engine.createGameObject();
    int angel_model = ren.modelManager().loadOBJ("assets/models/", "angel.obj", "angel.mtl");
    engine.giveComponents(angel_obj, TRANSFORM, MODEL);
    modelCS.useModel(angel_obj, angel_model, 0);


    // int soundobj = engine.createGameObject();
    // engine.giveComponent(soundobj, TRANSFORM);
    // int nier1 = engine.aengine().loadWav("assets/audio/nier2.wav");
    // int emitter = engine.aengine().createEmitter(nier1, transCS.getTransform(soundobj));
    // engine.aengine().playSound(emitter);
    // engine.aengine().listenerPosition(&transCS.getTransform(player_obj));

    int dirlight_id = ren.createDirlight();


    while (engine.running())
    {
        engine.beginFrame();

        // idk::Engine::threadpool.create(
        //     [&transCS, &angel_obj, &engine]()
        //     {
        //         transCS.getTransform(angel_obj).rotateY( 0.5f * engine.deltaTime() );
        //         // transCS.getTransform(angel_obj).translate(glm::vec3(0.0f, 0.0f, 0.1f * engine.deltaTime()));
        //     }
        // );

        // idk::Engine::threadpool.create(
        //     [&transCS, &terrain2_obj, &engine]()
        //     {
        //         transCS.getTransform(terrain2_obj).rotateY( 0.5f * engine.deltaTime() );
        //     }
        // );

        if (engine.eventManager().keylog().keyTapped(idk::Keycode::E))
        {
            engine.rengine().compileShaders();
        }

        engine.endFrame();
    }

    return 0;
}
