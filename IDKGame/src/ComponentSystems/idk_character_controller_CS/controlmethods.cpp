#include "controlmethods.h"
#include "../idk_transform_CS.h"


static constexpr float SWAY_SPEED_LOOK   = 0.002f;
static constexpr float SWAY_SPEED_MOVE   = 0.01f;
static constexpr float SWAY_DECAY_FACTOR = 0.001f;
static float roll_sway  = 0.0f;
static float pitch_sway = 0.0f;


void
controlmethods::player( int obj_id, idk::Engine &engine, CharacterController &controller )
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");

    idk::Camera &camera = engine.rengine().getCamera();
    idk::Transform &transform = tCS.getTransform(obj_id);
    idk::Keylog &keylog = engine.eventManager().keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(idk::Keycode::C))
        engine.eventManager().mouseCapture(!engine.eventManager().mouseCaptured());

    float speed = 24.32f;

    if (keylog.keyDown(idk::Keycode::LSHIFT))
        speed = 64.0f;

    camera.roll(-roll_sway);

    if (keylog.keyDown(idk::Keycode::ESCAPE))
    {
        engine.shutdown();
    }

    constexpr float MOVESPEED = 4.0f;

    if (keylog.keyDown(idk::Keycode::A))
        camera.translate(MOVESPEED*dtime*glm::vec3(-1.0f, 0.0f, 0.0f));
    if (keylog.keyDown(idk::Keycode::D))
        camera.translate(MOVESPEED*dtime*glm::vec3(+1.0f, 0.0f, 0.0f));
    if (keylog.keyDown(idk::Keycode::W))
        camera.translate(MOVESPEED*dtime*glm::vec3(0.0f, 0.0f, -1.0f));
    if (keylog.keyDown(idk::Keycode::S))
        camera.translate(MOVESPEED*dtime*glm::vec3(0.0f, 0.0f, +1.0f));


    if (keylog.keyDown(idk::Keycode::SPACE))
        camera.elevation(+MOVESPEED*dtime);
    if (keylog.keyDown(idk::Keycode::LCTRL))
        camera.elevation(-MOVESPEED*dtime);


    float decay = std::pow(SWAY_DECAY_FACTOR, dtime);
    roll_sway = idk::clamp(decay*roll_sway, -0.5f, 0.5f);

    if (engine.eventManager().mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.eventManager().mouseDelta();
        camera.pitch(-dmouse.y);
        camera.yaw(-dmouse.x);
        roll_sway -= speed * SWAY_SPEED_LOOK * dmouse.x;
    }

    camera.roll(roll_sway);
}

