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
    const int PHYSICS    = engine.registerCS<Physics_CS>("physics");
    const int GRABBABLE  = engine.registerCS<Grabbable_CS>("grabbable");
    const int POINTLIGHT = engine.registerCS<PointLight_CS>("pointlight");
    const int CHARCONTROL= engine.registerCS<CharacterController_CS>("charactercontrol");
    const int SPOTLIGHT  = engine.registerCS<SpotLight_CS>("spotlight");
    const int CAMERA     = engine.registerCS<Camera_CS>("camera");

    auto &transCS = engine.getCS<Transform_CS>(TRANSFORM);
    auto &modelCS = engine.getCS<Model_CS>(MODEL);
    auto &physCS  = engine.getCS<Physics_CS>(PHYSICS);
    auto &pointCS = engine.getCS<PointLight_CS>(POINTLIGHT);
    auto &spotCS  = engine.getCS<SpotLight_CS>(SPOTLIGHT);
    auto &grabCS  = engine.getCS<Grabbable_CS>(GRABBABLE);
    auto &charCS  = engine.getCS<CharacterController_CS>(CHARCONTROL);


    engine.registerModule<ImGui_Module>("imgui");
    ren.modelManager().loadIDKtexpak("assets/textures/diffuse.texpak",  true);
    ren.modelManager().loadIDKtexpak("assets/textures/specular.texpak", false);
    ren.modelManager().loadIDKtexpak("assets/textures/reflection.texpak", false);
    ren.getCamera().ylock(true);

    GLuint default_geometrypass = idk::gltools::compileProgram(
        "shaders/deferred/", "geometrypass.vs", "geometrypass.fs"
    );


    int terrain2_obj = engine.createGameObject();
    int terrain2_model = ren.modelManager().loadOBJ("assets/models/", "man.obj", "man.mtl");
    engine.giveComponents(terrain2_obj, TRANSFORM, MODEL);
    transCS.translate(terrain2_obj, glm::vec3(0.0f, 15.0f, 2.0f));
    modelCS.useModel(terrain2_obj, terrain2_model, default_geometrypass);


    int player_obj = engine.createGameObject();
    engine.giveComponents(player_obj, TRANSFORM, PHYSICS, CAMERA, CHARCONTROL);
    transCS.translate(player_obj, glm::vec3(0.0f, 5.0f, 2.0f));
    physCS.giveCapsuleCollider(player_obj);
    charCS.controlMethod(player_obj, controlmethods::player);


    int platform_obj = engine.createGameObject();
    int platform_model = ren.modelManager().loadOBJ("assets/models/", "hall.obj", "hall.mtl");
    engine.giveComponents(platform_obj, TRANSFORM, MODEL);
    modelCS.useModel(platform_obj, platform_model, default_geometrypass);


    int angel_obj = engine.createGameObject();
    int angel_model = ren.modelManager().loadOBJ("assets/models/", "angel.obj", "angel.mtl");
    engine.giveComponents(angel_obj, TRANSFORM, MODEL);
    modelCS.useModel(angel_obj, angel_model, default_geometrypass);


    int sphere_obj = engine.createGameObject();
    int sphere_model = ren.modelManager().loadOBJ("assets/models/", "sphere.obj", "sphere.mtl");
    engine.giveComponents(sphere_obj, TRANSFORM, MODEL);
    modelCS.useModel(sphere_obj, sphere_model, default_geometrypass);
    transCS.translate(sphere_obj, glm::vec3(0.0f, 0.0f, 10.0f));


    // int soundobj = engine.createGameObject();
    // engine.giveComponent(soundobj, TRANSFORM);
    // int nier1 = engine.aengine().loadWav("assets/audio/nier2.wav");
    // int emitter = engine.aengine().createEmitter(nier1, transCS.getTransform(soundobj));
    // engine.aengine().playSound(emitter);
    // engine.aengine().listenerPosition(&transCS.getTransform(player_obj));




    int dirlight_id = ren.createDirlight();

    // int spotlight_obj = engine.createGameObject();
    // engine.giveComponents(spotlight_obj, TRANSFORM, SPOTLIGHT);
    // glm::vec3 last_dir = ren.getCamera().front();


    while (engine.running())
    {
        engine.beginFrame();
        // m_audio_engine.update();

        idk::Engine::threadpool.push(
            [&transCS, &angel_obj, &engine]()
            {
                transCS.getTransform(angel_obj).rotateY( 0.5f * engine.deltaTime() );
                // transCS.getTransform(angel_obj).translate(glm::vec3(0.0f, 0.1f * engine.deltaTime(), 0.0f));
            }
        );

        idk::Engine::threadpool.push(
            [&transCS, &terrain2_obj, &engine]()
            {
                transCS.getTransform(terrain2_obj).rotateY( 0.5f * engine.deltaTime() );
            }
        );


        idk::Engine::threadpool.join();

        // auto &transform = transCS.getTransform(spotlight_obj);
        // transform = idk::Transform(glm::inverse(ren.getCamera().view()));
        // transform.localTranslate(glm::vec3(0.0f, 0.0f, 1.0f));

        // glm::vec3 front = glm::mat3(ren.getCamera().transform().modelMatrix()) * glm::vec3(0.0f, 0.0f, -1.0f);
        // glm::vec3 dir = (front - last_dir);
        // last_dir += 10.0f * engine.deltaTime() * dir;
        
        // spotCS.getSpotlight(spotlight_obj).direction = glm::vec4(last_dir, 0.0f);

        if (engine.eventManager().keylog().keyTapped(idk_keycode::E))
        {
            engine.rengine().compileShaders();
        }

        engine.endFrame();
    }

    return 0;
}
