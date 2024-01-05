#include "playercontroller_cs.hpp"
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>


static constexpr float SWAY_SPEED_LOOK   = 5.002f;
static constexpr float SWAY_SPEED_MOVE   = 5.01f;
static constexpr float SWAY_DECAY_FACTOR = 0.001f;
static constexpr float VELOCITY_DECAY    = 0.01f;
static float roll_sway  = 0.0f;
static float pitch_sway = 0.0f;

static constexpr float MOVESPEED       = 0.025f;
static constexpr float RUN_MULTIPLIER  = 1.2f;
static constexpr float WALK_MULTIPLIER = 0.6f;


glm::vec3 velocity(0.0f);


void idkg::CharacterController::update( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();


    int obj_id = m_obj_id;

    idk::Transform_CS &tCS   = engine.getCS<idk::Transform_CS>();
    idk::Camera_CS    &camCS = engine.getCS<idk::Camera_CS>();
    idk::Model_CS     &mCS   = engine.getCS<idk::Model_CS>();

    idk::Camera     &camera    = camCS.getCamera(ren, obj_id);
    // idk::Transform  &transform = tCS.getTransform(obj_id);
    idk::Keylog     &keylog    = engine.eventManager().keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(idk::Keycode::C))
        engine.eventManager().mouseCapture(!engine.eventManager().mouseCaptured());

    float speed = MOVESPEED*dtime;

    if (keylog.keyDown(idk::Keycode::LSHIFT))
    {
        speed *= RUN_MULTIPLIER;
    }

    else
    {
        speed *= WALK_MULTIPLIER;
    }

    if (keylog.keyDown(idk::Keycode::ESCAPE))
    {
        engine.shutdown();
    }


    // Translate gameobject with WASD keys
    // -----------------------------------------------------------------------------------------
    glm::vec3 right = glm::normalize(camera.right());
    glm::vec3 front = glm::normalize(glm::cross(right, -glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::vec3 delta = glm::vec3(0.0f);

    if (keylog.keyDown(idk::Keycode::A))     delta += 300.0f * dtime * -speed*right;
    if (keylog.keyDown(idk::Keycode::D))     delta += 300.0f * dtime * +speed*right;
    if (keylog.keyDown(idk::Keycode::W))     delta += 300.0f * dtime * +speed*front;
    if (keylog.keyDown(idk::Keycode::S))     delta += 300.0f * dtime * -speed*front;

    if (keylog.keyDown(idk::Keycode::SPACE)) delta.y += 300.0f * dtime * +speed;
    if (keylog.keyDown(idk::Keycode::LCTRL)) delta.y += 300.0f * dtime * -speed;

    bearing.x += 250.0f * delta.x;
    bearing.z += 250.0f * delta.z;
    bearing = glm::normalize(bearing);
    // transform.pointTowards(bearing + transform.position());

    velocity.y += delta.y;
    velocity *= 0.95f;


    static float alpha = 0.0f;

    alpha += 100.0f * glm::length2(delta / dtime);
    alpha = glm::clamp(alpha, 0.0f, 0.99f);
    alpha *= 0.975f;

    glm::vec3 motion = m_animator.getMotion(alpha, bearing, mCS.getAnimator(ren, m_obj_id));

    tCS.translate(obj_id, motion);

    // transform.translate(motion);
    // transform.translate(glm::vec3(0.0f, velocity.y, 0.0f));


    if (engine.eventManager().mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.eventManager().mouseDelta();
        camera.pitch(-dmouse.y);
        camera.yaw(-dmouse.x);
    }
}

