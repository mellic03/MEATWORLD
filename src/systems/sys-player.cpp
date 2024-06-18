#include "sys-player.hpp"

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>

#include <libidk/idk_log.hpp>


idk::EngineAPI *api_ptr;


class LegController
{
private:
    int m_player;
    int m_lh, m_lk, m_lf, m_rh, m_rk, m_rf;
    int m_right;

    float distAB = 0.6f;
    float distBC = 0.75f;

    bool m_left = true;

    glm::vec3 pos, lhpos, lkpos, lfpos, rhpos, rkpos, rfpos;
    glm::vec3 ltarget, rtarget;
    glm::vec3 m_motion = glm::vec3(0.0f);



public:

    LegController( int player )
    {
        using namespace idk;

        m_player = player;
        m_lf = ECS2::createGameObject("Foot-L");
        m_rf = ECS2::createGameObject("Foot-R");

        m_lh = ECS2::createGameObject("Hip-L");
        m_rh = ECS2::createGameObject("Hip-R");

        m_lk = ECS2::createGameObject("Knee-L");
        m_rk = ECS2::createGameObject("Knee-R");

        ECS2::giveComponent<TransformCmp>(m_lh);
        ECS2::giveComponent<TransformCmp>(m_lk);
        ECS2::giveComponent<TransformCmp>(m_lf);

        ECS2::giveComponent<TransformCmp>(m_rh);
        ECS2::giveComponent<TransformCmp>(m_rk);
        ECS2::giveComponent<TransformCmp>(m_rf);

        ECS2::giveChild(player, m_lh);
        ECS2::giveChild(m_lh, m_lk);
        ECS2::giveChild(m_lk, m_lf);

        ECS2::giveChild(player, m_rh);
        ECS2::giveChild(m_rh, m_rk);
        ECS2::giveChild(m_rk, m_rf);

        TransformSys::setPositionLocalspace(m_lh, glm::vec3(-0.25f, -0.0f, 0.0f));
        TransformSys::setPositionLocalspace(m_rh, glm::vec3(+0.25f, -0.0f, 0.0f));

        TransformSys::setPositionLocalspace(m_lf, glm::vec3(0.0f, -(distAB+distBC), -0.3f));
        TransformSys::setPositionLocalspace(m_rf, glm::vec3(0.0f, -(distAB+distBC), -0.3f));

        ECS2::giveComponent<IKCmp>(m_lf);
        ECS2::giveComponent<IKCmp>(m_rf);

    };


    void _stationary( float dt )
    {
        using namespace idk;

        glm::vec3 hit;

        if (PhysicsSys::raycast(lhpos, glm::vec3(0.0f, -1.0f, 0.0f), hit))
        {
            float dist1 = glm::distance(hit, lhpos);
            float dist2 = glm::distance(hit, lfpos);

            float dx = hit.x - lfpos.x;
            float dz = hit.z - lfpos.z;
            float distxz = sqrt(dx*dx + dz*dz);
            float dist3 = fabs(hit.y - lfpos.y);

            if (dist1 > (distAB+distBC))
            {
                ltarget = lhpos;
                ltarget.y -= 0.75f*(distAB+distBC);
            }

            else if (distxz > 0.5f)
            {
                ltarget = hit;
            }

            else if (dist3 > 0.0f)
            {
                ltarget.y = hit.y;
            }
        }

        if (PhysicsSys::raycast(rhpos, glm::vec3(0.0f, -1.0f, 0.0f), hit))
        {
            float dist1 = glm::distance(hit, rhpos);
            float dist2 = glm::distance(hit, rfpos);
    
            float dx = hit.x - rfpos.x;
            float dz = hit.z - rfpos.z;
            float distxz = sqrt(dx*dx + dz*dz);
            float dist3 = fabs(hit.y - rfpos.y);

            if (dist1 > (distAB+distBC))
            {
                rtarget = rhpos;
                rtarget.y -= 0.75f*(distAB+distBC);
            }

            else if (distxz > 0.5f)
            {
                rtarget = hit;
            }

            else if (dist3 > 0.0f)
            {
                rtarget.y = hit.y;
            }
        }
    }


    void _forward( float dt )
    {
        using namespace idk;

    }


    void _update( float dt )
    {
        using namespace idk;

        pos   = TransformSys::getPositionWorldspace(m_player);
        lhpos = TransformSys::getPositionWorldspace(m_lh);
        lfpos = TransformSys::getPositionWorldspace(m_lf);
        rhpos = TransformSys::getPositionWorldspace(m_rh);
        rfpos = TransformSys::getPositionWorldspace(m_rf);
        lkpos = TransformSys::getPositionWorldspace(m_lk);
        rkpos = TransformSys::getPositionWorldspace(m_rk);

        glm::vec3 front = TransformSys::getFront(m_player);

        glm::vec3 avg = 0.5f * (lfpos + rfpos);
                  avg.y = 0.0f;

        glm::vec3 xzpos = pos; xzpos.y = 0.0f;
        glm::vec3 delta = avg - xzpos;


        float desired = pos.y - 0.75f*(distAB+distBC);
        float current = 0.5f*(lfpos.y + rfpos.y);
              current = glm::max(current, glm::min(lfpos.y, rfpos.y));
        float error = current - desired;
        glm::vec3 E = 0.05f * glm::vec3(0.0f, error, 0.0f);


        TransformSys::translateWorldspace(m_player, 0.1f*dt*delta + E);

        TransformSys::FABRIK(3, m_lf, {distAB, distBC}, lhpos - front);
        TransformSys::FABRIK(3, m_rf, {distAB, distBC}, rhpos - front);

    }

    void update( float dt )
    {
        using namespace idk;

        _update(dt);

        m_motion = TransformSys::getData(m_player).delta;

        // if (glm::length(m_motion) < 0.001f)
        {
            _stationary(dt);
        }

        // else if (fabs(m_motion.z) > fabs(m_motion.x))
        // {
        //     _forward(dt);
        // }

        TransformSys::translateWorldspace(m_lf, 32.0f*dt*(ltarget - lfpos));
        TransformSys::translateWorldspace(m_rf, 32.0f*dt*(rtarget - rfpos));

    };


    void move( const glm::vec3 &v )
    {
        m_motion += v;
    };

};



void
idk::PlayerSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}




void
idk::PlayerSys::update( idk::EngineAPI &api )
{
    for (auto &cmp: idk::ECS2::getComponentArray<idk::PlayerControllerCmp>())
    {
        cmp.input(api);
        cmp.update(api);

        // static LegController L(cmp.obj_id);
        // L.update(api.getEngine().deltaTime());
    }

    for (auto &cmp: idk::ECS2::getComponentArray<idk::OLPlayerControllerCmp>())
    {
        if (cmp.obj_id == -1)
        {
            continue;
        }

        cmp.input(api);
        cmp.update(api);
    }

    for (auto &cmp: idk::ECS2::getComponentArray<idk::PlayerArmsCmp>())
    {
        cmp.update(api);
        cmp.draw(api);
    }
}



void
idk::PlayerControllerCmp::update( idk::EngineAPI &api )
{
    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    if (model_obj == -1)
    {
        model_obj = idk::ECS2::createGameObject("model");
        idk::ECS2::giveComponent<TransformCmp>(model_obj);

        idk::ECS2::giveChild(obj_id, model_obj);
        TransformSys::setPositionLocalspace(model_obj, glm::vec3(0.0f));
    }

    if (hinge_obj == -1)
    {
        hinge_obj = idk::ECS2::createGameObject("hinge");
        idk::ECS2::giveComponent<TransformCmp>(hinge_obj);

        idk::ECS2::giveChild(obj_id, hinge_obj);
        TransformSys::setPositionLocalspace(hinge_obj, glm::vec3(0.0f));
    }

    TransformSys::getData(obj_id).yaw = TransformSys::getData(hinge_obj).yaw;


    static float G = 9.8f;
    static glm::vec3 acc = glm::vec3(0.0f, 0.0f, 0.0f);
    static glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f);

    // auto &cmp = idk::ECS2::getComponent<idk::KinematicCapsuleCmp>(obj_id);

    // acc.y = -PhysicsConstants::G;

    // if (cmp.enabled)
    // {
    //     if (cmp.grounded)
    //     {
    //         vel.y *= 0.5f;

    //         if (K.keyDown(idk::Keycode::SPACE))
    //         {
    //             vel.y = jump_force*PhysicsConstants::G;
    //             // glm::vec3 f = glm::vec3(0.0f, jump_force*PhysicsConstants::G, 0.0f);
    //             // PhysicsSys::addForce(obj_id, f);
    //         }

    //         if (K.keyDown(idk::Keycode::LCTRL))
    //         {
    //             cmp.bottom = 0.5f;
    //         }

    //         else if (cmp.bottom < 0.75f)
    //         {
    //             cmp.bottom += 2.0f*dtime;
    //         }
    //     }

    //     vel = glm::clamp(vel, glm::vec3(-100.0f), glm::vec3(+100.0f));
    //     vel += dtime*acc;
    //     glm::vec3 dP = vel + 0.5f*dtime*acc;
    //     // PhysicsSys::addForce(obj_id, dP);
    // }

}



void
idk::PlayerControllerCmp::input( idk::EngineAPI &api )
{
    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    glm::vec3 delta = glm::vec3(0.0f);
    glm::vec3 up    = TransformSys::getUp(obj_id);
    glm::vec3 right = TransformSys::getRight(obj_id);
    glm::vec3 front = TransformSys::getFront(obj_id);


    if (K.keyDown(idk::Keycode::A)) delta -= right;
    if (K.keyDown(idk::Keycode::D)) delta += right;
    if (K.keyDown(idk::Keycode::W)) delta += front;
    if (K.keyDown(idk::Keycode::S)) delta -= front;

    delta.y = 0.0f;

    if (fabs(delta.x) > 0.01f || fabs(delta.z) > 0.01f)
    {
        delta = 0.0025f * walk_speed * glm::normalize(delta);
    }

    TransformSys::translateWorldspace(obj_id, delta);
    // PhysicsSys::addForce(obj_id, delta);


    if (K.keyTapped(idk::Keycode::ESCAPE))
    {
        bool captured = events.mouseCaptured();
        events.mouseCapture(!captured);
    }

    if (events.mouseCaptured())
    {
        glm::vec2 dmouse = -events.mouseDelta();
        float mouse_speed = 0.1f;

        TransformSys::yaw(hinge_obj, mouse_speed*dmouse.x);
        TransformSys::pitch(hinge_obj, mouse_speed*dmouse.y);
    }
}




void
idk::OLPlayerControllerCmp::update( idk::EngineAPI &api )
{
    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    if (model_obj == -1)
    {
        model_obj = idk::ECS2::createGameObject("model");
        idk::ECS2::giveComponent<TransformCmp>(model_obj);

        idk::ECS2::giveChild(obj_id, model_obj);
        TransformSys::setPositionLocalspace(model_obj, glm::vec3(0.0f));
    }

    if (hinge_obj == -1)
    {
        hinge_obj = idk::ECS2::createGameObject("hinge");
        idk::ECS2::giveComponent<TransformCmp>(hinge_obj);

        idk::ECS2::giveChild(obj_id, hinge_obj);
        TransformSys::setPositionLocalspace(hinge_obj, glm::vec3(0.0f));
    }

    TransformSys::getData(obj_id).yaw = TransformSys::getData(hinge_obj).yaw;


    static float G = 9.8f;
    static glm::vec3 acc = glm::vec3(0.0f, 0.0f, 0.0f);
    static glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f);
}



void
idk::OLPlayerControllerCmp::input( idk::EngineAPI &api )
{
    if (m_controller == nullptr)
    {
        m_controller = new LegController(this->obj_id);
    }

    m_controller->update(api.getEngine().deltaTime());
}



void
idk::PlayerArmsCmp::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    float dt  = api.getEngine().deltaTime();

    // if (shoulder_obj == -1 || elbow_obj == -1 || hand_obj == -1)
    // {
    //     return;
    // }

    int parent = ECS2::getParent(obj_id);

    glm::vec3 shoulder_pos = TransformSys::getPositionWorldspace(obj_id);

    glm::vec3 rest_dir   = glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f));
    glm::vec3 rest_pos   = shoulder_pos + (0.8f*(distAB+distBC)) * rest_dir;

    glm::vec3 ray_origin = shoulder_pos;
    glm::vec3 ray_dir    = TransformSys::getFront(parent);
              ray_dir    = glm::normalize(ray_dir);
    glm::vec3 hit        = rest_pos;


    if (PhysicsSys::raycast(ray_origin, ray_dir, hit))
    {
        float dist1 = glm::distance(ray_origin, hit);

        if (dist1 < distAB+distBC)
        {
        }

        else
        {
            hit = rest_pos;
        }
    }
    else
    {
        hit = rest_pos;
    }

    hand_pos += 16.0f * dt * (hit - hand_pos);

    glm::vec3 pole_target = shoulder_pos - TransformSys::getFront(parent) + glm::vec3(0.0f, -0.25f, 0.0f);


    TransformSys::FABRIK(
        shoulder_pos, elbow_pos, hand_pos, pole_target, distAB, distBC
    );

}






void
idk::PlayerArmsCmp::draw( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    glm::vec3 shoulder_pos = TransformSys::getPositionWorldspace(obj_id);

    ren.drawCapsule(shoulder_pos, elbow_pos, 0.025f);
    ren.drawCapsule(elbow_pos, hand_pos, 0.025f);

    // glm::vec3 A = glm::vec3(0.0f);
    // glm::vec3 B = elbow_pos - shoulder_pos;
    // glm::mat4 V = glm::inverse(glm::lookAt(A, B, glm::vec3(0.0f, 1.0f, 0.0f)));
    // glm::mat4 M = TransformSys::getWorldMatrix(obj_id);
    // glm::mat4 R = glm::inverse(extractRotation(M)) * V;

    // TransformSys::getData(obj_id).rotation = glm::normalize(glm::quat_cast(R));
    // // TransformSys::lookTowards(obj_id, elbow_pos, 0.5f);

}





size_t
idk::PlayerControllerCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, hinge_obj);
    n += idk::streamwrite(stream, cam_obj);
    n += idk::streamwrite(stream, model_obj);
    n += idk::streamwrite(stream, walk_speed);
    n += idk::streamwrite(stream, run_speed);
    n += idk::streamwrite(stream, jump_force);
    return n;
}


size_t
idk::PlayerControllerCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, hinge_obj);
    n += idk::streamread(stream, cam_obj);
    n += idk::streamread(stream, model_obj);
    n += idk::streamread(stream, walk_speed);
    n += idk::streamread(stream, run_speed);
    n += idk::streamread(stream, jump_force);
    return n;
}


void
idk::PlayerControllerCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::PlayerControllerCmp::onObjectAssignment";

    auto &cmp = ECS2::getComponent<PlayerControllerCmp>(obj_id);

    cmp.walk_speed = 2.0f;
    cmp.cam_obj    = ECS2::createGameObject("camera", false);
    cmp.hinge_obj  = idk::ECS2::createGameObject("hinge", false);


    ECS2::giveComponent<TransformCmp>(cmp.cam_obj);
    ECS2::giveComponent<TransformCmp>(cmp.hinge_obj);
    ECS2::giveComponent<SmoothFollowCmp>(cmp.hinge_obj);


    auto &fcmp = ECS2::getComponent<SmoothFollowCmp>(cmp.hinge_obj);
    fcmp.anchor_id = obj_id;
    fcmp.speed     = 32.0f;


    ECS2::giveChild(cmp.hinge_obj, cmp.cam_obj);

    ECS2::giveComponent<CameraCmp>(cmp.cam_obj);
    TransformSys::setPositionLocalspace(cmp.cam_obj, glm::vec3(0.5f, 0.3f, 1.5f));
    ECS2::getComponent<CameraCmp>(cmp.cam_obj).cam_id = api.getRenderer().activeCamera();

}


void
idk::PlayerControllerCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::PlayerControllerCmp::onObjectDeassignment";
}


void
idk::PlayerControllerCmp::onObjectCopy( int src_obj, int dst_obj )
{
    IDK_ASSERT("Cannot copy idk::PlayerControllerCmp!", false);
}







size_t
idk::PlayerArmsCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, shoulder_obj);
    n += idk::streamwrite(stream, elbow_obj);
    n += idk::streamwrite(stream, hand_obj);
    n += idk::streamwrite(stream, distAB);
    n += idk::streamwrite(stream, distBC);
    return n;
}


size_t
idk::PlayerArmsCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, shoulder_obj);
    n += idk::streamread(stream, elbow_obj);
    n += idk::streamread(stream, hand_obj);
    n += idk::streamread(stream, distAB);
    n += idk::streamread(stream, distBC);
    return n;
}


void
idk::PlayerArmsCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PlayerArmsCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PlayerArmsCmp::onObjectCopy( int src_obj, int dst_obj )
{

}












size_t
idk::ArmCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, partner_id);
    n += idk::streamwrite(stream, is_left);
    n += idk::streamwrite(stream, rest_target);
    n += idk::streamwrite(stream, ray_target);
    n += idk::streamwrite(stream, pole_target);
    n += idk::streamwrite(stream, distAB);
    n += idk::streamwrite(stream, distBC);
    n += idk::streamwrite(stream, stride);
    n += idk::streamwrite(stream, step_speed);
    n += idk::streamwrite(stream, step_height);
    n += idk::streamwrite(stream, elbow_pos);
    n += idk::streamwrite(stream, hand_pos);
    n += idk::streamwrite(stream, target_pos);
    return n;
}


size_t
idk::ArmCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, partner_id);
    n += idk::streamread(stream, is_left);
    n += idk::streamread(stream, rest_target);
    n += idk::streamread(stream, ray_target);
    n += idk::streamread(stream, pole_target);
    n += idk::streamread(stream, distAB);
    n += idk::streamread(stream, distBC);
    n += idk::streamread(stream, stride);
    n += idk::streamread(stream, step_speed);
    n += idk::streamread(stream, step_height);
    n += idk::streamread(stream, elbow_pos);
    n += idk::streamread(stream, hand_pos);
    n += idk::streamread(stream, target_pos);

    return n;
}


void
idk::ArmCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::ArmCmp::onObjectAssignment";
}


void
idk::ArmCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::ArmCmp::onObjectDeassignment";
}


void
idk::ArmCmp::onObjectCopy( int src_obj, int dst_obj )
{

}



