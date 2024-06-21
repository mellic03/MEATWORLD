#include "sys-player.hpp"

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>

#include <libidk/idk_log.hpp>


idk::EngineAPI *api_ptr;


class CharacterLeg
{
private:
    int m_hip, m_knee, m_foot;
    int m_look, m_model;

    float distAB = 0.6f;
    float distBC = 0.75f;
    float m_desired = 0.75f*(distAB+distBC);

    glm::vec3 m_target = glm::vec3(0.0f);
    glm::vec3 curr_target = glm::vec3(0.0f);
    glm::vec3 next_target = glm::vec3(0.0f);

    std::string m_name = "";

public:
    glm::vec3 m_restdir;
    glm::vec3 m_hip_pos;
    glm::vec3 m_foot_pos;
    float m_alpha = 0.0f;


    CharacterLeg() {  };

    CharacterLeg( int player, const glm::vec3 &origin, const glm::vec3 &restdir, const std::string &name )
    {
        using namespace idk;

        m_name = name;
        m_restdir = restdir;

        m_hip  = ECS2::createGameObject(name + "-Hip");
        m_knee = ECS2::createGameObject(name + "-Knee");
        m_foot = ECS2::createGameObject(name + "-Foot");

        ECS2::giveComponent<TransformCmp>(m_knee);
        ECS2::giveComponent<TransformCmp>(m_hip);
        ECS2::giveComponent<TransformCmp>(m_foot);

        ECS2::giveChild(player, m_hip);
        ECS2::giveChild(m_hip,  m_knee);
        ECS2::giveChild(m_knee, m_foot);
    
        TransformSys::setPositionLocalspace(m_hip, origin);
        TransformSys::setPositionLocalspace(m_foot, glm::vec3(0.0f, -(distAB+distBC), -0.3f));

        ECS2::giveComponent<IKCmp>(m_foot);


        int parents[3] = { m_hip, m_knee, -1 };
        int targets[3] = { m_knee, m_foot, -1 };
        float offsets[3] = { 0.0f, distAB, distBC };

        for (int i=0; i<2; i++)
        {
            m_look  = ECS2::createGameObject(name + "-look");
            m_model = ECS2::createGameObject(name + "-model");

            parents[i+1] = m_look;

            ECS2::giveComponent<TransformCmp>(m_look);
            ECS2::giveComponent<LookTowardCmp>(m_look);
            ECS2::giveComponent<TransformCmp>(m_model);
            ECS2::giveComponent<ModelCmp>(m_model);
            TransformSys::pitch(m_model, 90.0f);
    
            ECS2::getComponent<LookTowardCmp>(m_look).target_id = targets[i];

            ModelSys::assignModel(m_model, "assets/models/muscle-arm.idkvi");

            ECS2::giveChild(parents[i], m_look);
            ECS2::giveChild(m_look, m_model);

            TransformSys::setPositionLocalspace(m_look, glm::vec3(0.0f, 0.0f, offsets[i]));
            TransformSys::setPositionLocalspace(m_model, glm::vec3(0.0f));
            TransformSys::getData(m_model).scale  = 0.15f;
            TransformSys::getData(m_model).scale3 = glm::vec3(1.0f, 0.6f, 1.0f);
        }
    }


    bool _raycast( int player, const glm::vec3 &front )
    {
        using namespace idk;

        glm::vec3 player_pos = TransformSys::getPositionWorldspace(player);
        // glm::vec3 front      = TransformSys::getFront(player);
        glm::vec3 hit;

        if (PhysicsSys::raycast(m_hip_pos+front, glm::vec3(0.0f, -1.0f, 0.0f), hit))
        {
            float dist0 = glm::distance(curr_target, hit);
            float dist1 = glm::distance(m_foot_pos, player_pos);

            // if (m_hip_pos.y - hit.y > distAB+distBC)
            // {
            //     curr_target = m_hip_pos - glm::vec3(0.0f, 0.75*(distAB+distBC), 0.0f);
            //     next_target = m_hip_pos - glm::vec3(0.0f, 0.75*(distAB+distBC), 0.0f);
            //     return true;
            // }

            if (abs(curr_target.y - hit.y) > m_desired)
            {
                curr_target = m_hip_pos + glm::vec3(0.0f, -m_desired, 0.0f);
            }

            else if (dist0 > 0.5f)
            {
                curr_target = next_target;
                next_target = hit;
                return true;
            }

        }

        return false;
    }


    void update( int player, float dt, glm::vec2 motion2, float offset )
    {
        using namespace idk;

        glm::vec3 player_pos = TransformSys::getPositionWorldspace(player);

        glm::vec3 front = TransformSys::getFront(player);
        m_hip_pos   = TransformSys::getPositionWorldspace(m_hip);
        m_foot_pos  = TransformSys::getPositionWorldspace(m_foot);


        // if (glm::length2(motion2) > 0.00001f)
        {
            _raycast(player, glm::vec3(motion2.x, 0.0f, motion2.y));
            // m_alpha += glm::length(motion2);
        }
    
        // while (m_alpha + offset > M_PI)
        // {
        //     m_alpha -= M_PI;
        // }
    
        // while (m_alpha + offset < -M_PI)
        // {
        //     m_alpha += M_PI;
        // }

        // float dy = sin(m_alpha + offset);
  

        TransformSys::FABRIK(3, m_foot, {distAB, distBC}, m_hip_pos - glm::vec3(0.0f, distAB, 0.0f) - front);
        TransformSys::moveTowards(m_foot, curr_target, 32.0f*dt);
    }

};


class LegController
{
private:
    int m_player;
    int m_lh, m_lk, m_lf, m_rh, m_rk, m_rf;


    CharacterLeg m_left, m_right;

    glm::vec3 pos, lhpos, lkpos, lfpos, rhpos, rkpos, rfpos;
    glm::vec3 ltarget, rtarget;
    glm::vec3 m_motion = glm::vec3(0.0f);



public:
    float distAB = 0.6f;
    float distBC = 0.75f;
    float m_desired = 0.75f*(distAB+distBC);
    float m_potential = 0.0f;

    LegController( int player )
    {
        using namespace idk;

        m_player = player;
    
        glm::vec3 left_pos  = glm::vec3(-0.25f, 0.0f, 0.0f);
        glm::vec3 right_pos = glm::vec3(+0.25f, 0.0f, 0.0f);

        glm::vec3 left_dir  = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right_dir = glm::vec3(0.0f, 0.0f, +1.0f);

        m_left  = CharacterLeg(player, left_pos, left_dir, "Left");
        m_right = CharacterLeg(player, right_pos, right_dir, "Right");
    };


    void update( float dt )
    {
        using namespace idk;

        m_motion = TransformSys::getData(m_player).delta;
        glm::vec2 motion2 = glm::vec2(m_motion.x, m_motion.z);

        m_left.update(m_player, dt, motion2, 0.0f);
        m_right.update(m_player, dt, motion2, M_PI/2.0f);


        glm::vec3 root_pos = TransformSys::getPositionWorldspace(m_player);
        glm::vec3 foot_avg = 0.5f * (m_left.m_foot_pos + m_right.m_foot_pos);
        glm::vec3 delta    = glm::vec3(1.0f, 0.0f, 1.0f) * (foot_avg - root_pos);


        float desired = root_pos.y - m_desired;
        float current = foot_avg.y;
              current = glm::max(current, glm::min(m_left.m_foot_pos.y, m_right.m_foot_pos.y));
        float error = current - desired;
        glm::vec3 E = 0.05f * glm::vec3(0.0f, error, 0.0f);

        TransformSys::translateWorldspace(m_player, 0.1f*dt*delta + E);
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


    if (m_controller == nullptr)
    {
        m_controller = new LegController(obj_id);
    }

    if (K.keyDown(Keycode::LCTRL))
    {
        m_controller->m_desired = 0.65f * 0.75f*(m_controller->distAB+m_controller->distBC);
    }

    else
    {
        m_controller->m_desired = 0.75f*(m_controller->distAB+m_controller->distBC);
    }

    m_controller->update(dtime);

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


    // glm::vec3 pos = TransformSys::getPositionWorldspace(obj_id);
    // glm::vec3 hit;

    // if (PhysicsSys::raycast(pos, glm::vec3(0.0f, -1.0f, 0.0f), hit))
    // {
    //     float dist = glm::distance(pos, hit);
    //     if (dist > m_controller->m_desired)
    //     {
    //         float dy = glm::min(dist, 0.05f);
    //         TransformSys::translateWorldspace(obj_id, glm::vec3(0.0f, dy, 0.0f));
    //     }
    // }


    // if (ECS2::hasComponent<KinematicCapsuleCmp>(obj_id) == false)
    // {
    //     idk::ECS2::giveComponent<idk::KinematicCapsuleCmp>(obj_id);
    // }

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
    //             glm::vec3 f = glm::vec3(0.0f, jump_force*PhysicsConstants::G, 0.0f);
    //             PhysicsSys::addForce(obj_id, f);
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

    //     // TransformSys::translateWorldspace(obj_id, glm::vec3(0.0f, -0.02f, 0.0f));
    //     PhysicsSys::addForce(obj_id, dP);
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


    // if (ECS2::hasComponent<KinematicCapsuleCmp>(obj_id) == false)
    // {
    //     idk::ECS2::giveComponent<idk::KinematicCapsuleCmp>(obj_id);
    // }

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
    TransformSys::getData(obj_id).position.y = 0.8f;


    cmp.walk_speed = 4.0f;
    cmp.cam_obj    = ECS2::createGameObject("camera", false);
    cmp.hinge_obj  = idk::ECS2::createGameObject("hinge", false);


    // ECS2::giveComponent<TransformCmp>(cmp.cam_obj);
    // ECS2::giveComponent<TransformCmp>(cmp.hinge_obj);
    ECS2::giveComponent<SmoothFollowCmp>(cmp.hinge_obj);


    auto &fcmp = ECS2::getComponent<SmoothFollowCmp>(cmp.hinge_obj);
    fcmp.anchor_id = obj_id;
    fcmp.speed     = 32.0f;


    ECS2::giveChild(cmp.hinge_obj, cmp.cam_obj);

    ECS2::giveComponent<CameraCmp>(cmp.cam_obj);
    TransformSys::setPositionLocalspace(cmp.cam_obj, glm::vec3(0.5f, 0.3f, 1.5f));
    ECS2::getComponent<CameraCmp>(cmp.cam_obj).cam_id = api.getRenderer().activeCamera();




    cmp.model_obj = ECS2::createGameObject("model", false);
    // ECS2::giveComponent<TransformCmp>(cmp.model_obj);
    ECS2::giveComponent<ModelCmp>(cmp.model_obj);

    ECS2::giveChild(obj_id, cmp.model_obj);
    TransformSys::setPositionLocalspace(cmp.model_obj, glm::vec3(0.0f, 0.5f, -0.15f));
    TransformSys::getData(cmp.model_obj).yaw = -M_PI;

    ModelSys::assignModel(cmp.model_obj, "assets/models/head.idkvi");
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







void
idk::OLPlayerControllerCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::OLPlayerControllerCmp::onObjectAssignment";

    auto &cmp = ECS2::getComponent<OLPlayerControllerCmp>(obj_id);

    cmp.walk_speed = 4.0f;
    cmp.model_obj  = ECS2::createGameObject("model", false);
    // cmp.emitter_obj = ECS2::createGameObject("emitter", false);


    // ECS2::giveChild(obj_id, cmp.emitter_obj);
    // TransformSys::setPositionLocalspace(cmp.emitter_obj, glm::vec3(0.0f));
    // ECS2::giveComponent<ParticleCmp>(cmp.emitter_obj);
    // ECS2::getComponent<ParticleCmp>(cmp.emitter_obj).desc.scale = 0.0f;
    // ECS2::getComponent<ParticleCmp>(cmp.emitter_obj).desc.velocity.z = -15.0f;

    ECS2::giveComponent<TransformCmp>(cmp.model_obj);
    ECS2::giveComponent<ModelCmp>(cmp.model_obj);

    ECS2::giveChild(obj_id, cmp.model_obj);
    TransformSys::setPositionLocalspace(cmp.model_obj, glm::vec3(0.0f, 0.5f, -0.15f));
    TransformSys::getData(cmp.model_obj).yaw = -M_PI;

    ModelSys::assignModel(cmp.model_obj, "assets/models/head.idkvi");

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



