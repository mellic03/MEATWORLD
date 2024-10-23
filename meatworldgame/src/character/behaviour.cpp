#include "behaviour.hpp"
#include "character.hpp"
#include "../world/world.hpp"
#include "../physics/rigidbody.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKIO/IDKIO.hpp>

#include <libidk/idk_transformIK.hpp>


using namespace meat;



void
BehaviourGravity::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();

    if (C->isGrounded() == false)
    {
        C->addForce(glm::vec3(0.0f, -idk::PhysicsConstants::G, 0.0f));
    }
}


void
BehaviourCollideWithTerrain::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();

    glm::vec3  vel = C->getVelocity();
    glm::vec3  acc = C->getAcc();
    glm::vec3 &pos = idk::TransformSys::getLocalPosition(C->getID());
    float      h   = idk::TerrainRenderer::heightQuery(pos.x, pos.z);

    float overlap = h - (pos.y - C->getHeight());

    if (overlap > 0.0f)
    {
        m_airtime = 0.0f;
        C->setGrounded(true);
    }

    else
    {
        m_airtime += dt;
    }

    if (m_airtime > dt_threshold)
    {
        C->setGrounded(false);
    }

    if (C->isGrounded())
    {
        pos.y = h + C->getHeight();
    }

}



void
BehaviourPushBodies::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();

    auto *body = world.nearestPhysicsBody(C->getPosition());

    if (body)
    {
        world.textPrompt("[E] push");

        if (idkio::keyDown(idk::Keycode::E))
        {
            body->applyLinear(2.0f * C->getFront());
        
            auto tau = meat::compute_torque(body->getPosition(), C->getPosition() + C->getFront(), 2.0f*C->getFront());
            body->applyAngular(tau);
        }
    }

}



// void
// BehaviourRenderSphere::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
// {
//     glm::vec3 pos = idk::TransformSys::getWorldPosition(C->getID());
//     api.getRenderer().drawSphere(pos, 1.0f);
// }



// void
// BehaviourLimbs::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
// {
//     auto &ren = api.getRenderer();

//     glm::vec3 &pos = idk::TransformSys::getLocalPosition(C->getID());
//     float height = idk::TerrainRenderer::heightQuery(pos.x, pos.z);

//     glm::vec3 target = glm::vec3(pos.x, height, pos.z);

//     if (glm::distance(m_prev, target) > 0.75f)
//     {
//         m_prev = target;
//     }

//     m_joints[0] = pos;
//     m_joints[2] = glm::mix(m_joints[2], m_prev, 0.1f);

//     idk::IK::FABRIK(m_joints, m_dists, 2.5f, 1);

//     ren.drawCapsule(m_joints[0], m_joints[1], 0.2f);
//     ren.drawCapsule(m_joints[1], m_joints[2], 0.2f);
//     ren.drawSphere(m_joints[2], 0.2f);

//     // pos.y = 1.75f + idk::TerrainRenderer::heightQuery(pos.x, pos.z);
// }



// void
// BehaviourMoveToPlayer::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
// {
//     glm::vec3 pos = idk::TransformSys::getLocalPosition(C->getID());

//     float dist2 = glm::distance2(pos, glm::vec3(0.0f));

//     if (m_moving == false && dist2 > 100.0f)
//     {
//         m_target = glm::vec3(0.0f);
//         m_moving = true;
//     }

//     else if (dist2 <= 0.01f)
//     {
//         m_moving = false;
//     }

//     else if (m_moving == true)
//     {
//         C->moveTo(m_target);
//     }
// }


void
BehaviourPlayerKeyInput::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();
    auto  kd = glm::vec3(0.0f);
    float speed = C->getActorDesc().walk_spd;

    if (idkio::keyDown(idk::Keycode::LSHIFT)) speed = C->getActorDesc().run_spd;

    if (idkio::keyDown(idk::Keycode::SPACE)) kd.y += speed;
    if (idkio::keyDown(idk::Keycode::LCTRL)) kd.y -= speed;

    if (idkio::keyDown(idk::Keycode::A)) kd.x -= speed;
    if (idkio::keyDown(idk::Keycode::D)) kd.x += speed;
    if (idkio::keyDown(idk::Keycode::W)) kd.z -= speed;
    if (idkio::keyDown(idk::Keycode::S)) kd.z += speed;

    C->move(dt*kd);
}


void
BehaviourPlayerZoom::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();

    if (idkio::mouseCaptured() == false)
    {
        return;
    }

    float  cam_fov = idk::CameraSys::getFov(C->getAttachID());
    float &offset  = idk::CameraSys::getFovOffset(C->getAttachID());
    float  alpha   = glm::clamp(dt/0.15f, 0.0f, 1.0f);

    if (idkio::keyTapped(idk::Keycode::Z))
    {
        m_zoomlevel = (m_zoomlevel+1) % 3;
    }

    float target = 0.8f * cam_fov * (float(m_zoomlevel) / 2.0f);
    offset = glm::mix(offset, target, alpha);

    glm::vec3 &pos = idk::TransformSys::getLocalPosition(C->getAttachID());
    pos.z -= idkio::mouseWheelDelta();
    pos.z = glm::clamp(pos.z, 0.0f, 255.0f);
}


void
BehaviourPlayerMouseInput::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();

    if (idkio::mouseCaptured())
    {
        glm::vec2 md = glm::vec2(-1) * 0.0005f * idkio::mouseDelta();
        C->look(md);
    }
}
