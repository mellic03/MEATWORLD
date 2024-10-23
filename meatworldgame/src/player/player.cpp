#include "player.hpp"
#include "../world/world.hpp"
#include "../item/flashlight.hpp"
#include "../item/inventory.hpp"
#include "../character/behaviour.hpp"
#include "../character/weapon.hpp"
#include "../vehicle/vehicle.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>
#include <IDKIO/IDKIO.hpp>

#include <libidk/idk_math.hpp>


using namespace meat;



CharacterFreecam::CharacterFreecam( World &world, const glm::vec3 &pos, float yaw )
:   ActorBase(world, "Freecam", pos, yaw)
{
    {
        m_hinge_obj = idk::ECS2::createGameObject("Hinge", false);

        idk::ECS2::removeChild(m_root_obj, m_attach_obj);
        idk::ECS2::giveChild(m_root_obj, m_hinge_obj);
        idk::ECS2::giveChild(m_hinge_obj, m_attach_obj);

        idk::TransformSys::getLocalPosition(m_hinge_obj) *= 0.0f;
        idk::TransformSys::getLocalPosition(m_attach_obj) *= 0.0f;

        setActorDesc({0.0f, 16.0f, 32.0f});

        idk::ECS2::setGameObjectName(m_attach_obj, "Camera");
        idk::ECS2::giveComponent<idk::CameraCmp>(m_attach_obj);
        idk::ECS2::giveComponent<idk::AudioListenerCmp>(m_attach_obj);

        giveItem<ItemFlashlight>();
        giveItem<ItemInventory>(world);
    }


    giveBehaviour<BehaviourPlayerKeyInput>();
    giveBehaviour<BehaviourPlayerMouseInput>();
    giveBehaviour<BehaviourPlayerZoom>();
    giveBehaviour<BehaviourDriveVehicle>();
}



CharacterPlayerFPS::CharacterPlayerFPS( World &world, const glm::vec3 &pos, float yaw )
:   ActorBase(world, "Player", pos, yaw)
{
    {
        m_hinge_obj = idk::ECS2::createGameObject("Hinge", false);

        idk::ECS2::removeChild(m_root_obj, m_attach_obj);
        idk::ECS2::giveChild(m_root_obj, m_hinge_obj);
        idk::ECS2::giveChild(m_hinge_obj, m_attach_obj);

        idk::TransformSys::getLocalPosition(m_hinge_obj) *= 0.0f;
        idk::TransformSys::getLocalPosition(m_attach_obj) *= 0.0f;

        setActorDesc({0.0f, 16.0f, 32.0f});

        idk::ECS2::setGameObjectName(m_attach_obj, "Camera");
        idk::ECS2::giveComponent<idk::CameraCmp>(m_attach_obj);
        idk::ECS2::giveComponent<idk::AudioListenerCmp>(m_attach_obj);

        setActorDesc({1.75f, 2.0f, 4.0f});

        idk::ECS2::setGameObjectName(m_root_obj, "Player");

        giveItem<ItemFlashlight>();
        giveItem<ItemInventory>(world);
        giveItem<ItemWeaponInventory>();
    }


    giveBehaviour<BehaviourPlayerKeyInput>();
    giveBehaviour<BehaviourPlayerMouseInput>();
    giveBehaviour<BehaviourPlayerZoom>();

    giveBehaviour<BehaviourGravity>();
    giveBehaviour<BehaviourCollideWithTerrain>();
    giveBehaviour<BehaviourDriveVehicle>();
    giveBehaviour<BehaviourPushBodies>();


    // const auto callback1 = [&world]()
    // {
    //     return (idkio::mouseCaptured()) && (idkio::keyTapped(idk::Keycode::E));
    // };

    // const auto callback2 = [this]()
    // {
    //     getItem<ItemInventory>()->open();
    //     return BNode::SUCCESS;
    // };


    // auto *node = new BNodeSequence({
    //     new BNodeCondition(callback1),
    //     new BNodeAction(callback2)
    // });

    // setBehaviour(node);
}



// void
// CharacterPlayerFPS::update( idk::EngineAPI &api, meat::World &world )
// {
//     auto *I = getItem<ItemInventory>();

//     if (idkio::keyTapped(idk::Keycode::E))
//     {
//         I->open();
//     }
// }




// BNode::Status
// BehaviourUserKeyInput::update( idk::EngineAPI &api, meat::World &world )
// {
//     float dt = api.dtime();
//     auto  *C = m_actor;

//     glm::vec3 kd = glm::vec3(0.0f);

//     if (idk::IO::keyDown(idk::Keycode::A)) kd.x -= dt;
//     if (idk::IO::keyDown(idk::Keycode::D)) kd.x += dt;
//     if (idk::IO::keyDown(idk::Keycode::W)) kd.z -= dt;
//     if (idk::IO::keyDown(idk::Keycode::S)) kd.z += dt;

//     if (idk::IO::keyDown(idk::Keycode::LSHIFT))
//     {
//         C->run(kd);
//     }

//     else
//     {
//         C->walk(kd);
//     }

//     return BNode::SUCCESS;
// }



// BNode::Status
// BehaviourUserMouseInput::update( idk::EngineAPI &api, meat::World &world )
// {
//     glm::vec2 md = glm::vec2(-1) * 0.0005f * idk::IO::mouseDelta();

//     m_actor->look(md);
//     return BNode::SUCCESS;
// }




// BNode::Status
// BNodeGravity::update( idk::EngineAPI &api, meat::World &world )
// {
//     float dt = api.dtime();
//     auto *C  = m_actor;

//     if (C->isGrounded() == false)
//     {
//         C->addForce(glm::vec3(0.0f, -0.1f*idk::PhysicsConstants::G, 0.0f));
//     }

//     return BNode::SUCCESS;
// }



// BNode::Status
// BNodeTerrainCollision::update( idk::EngineAPI &api, meat::World &world )
// {
//     float dt = api.dtime();
//     auto *C  = m_actor;


//     glm::vec3  vel = C->getVelocity();
//     glm::vec3  acc = C->getAcc();
//     glm::vec3 &pos = idk::TransformSys::getLocalPosition(C->getID());
//     float      h   = idk::TerrainRenderer::heightQuery(pos.x, pos.z);

//     float overlap = h - (pos.y - C->getHeight());

//     if (overlap > 0.0f)
//     {
//         m_airtime = 0.0f;
//         C->setGrounded(true);
//     }

//     else
//     {
//         m_airtime += dt;
//     }

//     if (m_airtime > dt_threshold)
//     {
//         C->setGrounded(false);
//     }

//     if (C->isGrounded())
//     {
//         pos.y = h + C->getHeight();
//     }


//     return BNode::SUCCESS;
// }





// CharacterPlayerFPS::CharacterPlayerFPS( World &world, const glm::vec3 &pos, float yaw )
// :   CharacterFreecam(world, pos, yaw)
// {
//     setActorDesc({1.75f, 2.0f, 4.0f});

//     idk::ECS2::setGameObjectName(m_root_obj, "Player");
//     giveItem<ItemFlashlight>();
//     giveItem<ItemWeaponInventory>();

//     // giveBehaviour<BehaviourPlayerKeyInput>();
//     // giveBehaviour<BehaviourPlayerMouseInput>();
//     // giveBehaviour<BehaviourPlayerZoom>();

//     // giveBehaviour<BehaviourGravity>();
//     // giveBehaviour<BehaviourCollideWithTerrain>();
//     // giveBehaviour<BehaviourDriveVehicle>();



//     m_sounds0[0] = idk::AudioSystem::createEmitter("assets/audio/dialogue/omg-boat1.wav");
//     m_sounds0[1] = idk::AudioSystem::createEmitter("assets/audio/dialogue/hello-boat.wav");
//     m_sounds0[2] = idk::AudioSystem::createEmitter("assets/audio/dialogue/ily-boat.wav");
//     m_sounds0[3] = idk::AudioSystem::createEmitter("assets/audio/dialogue/later-boat.wav");

//     m_sounds1[0] = idk::AudioSystem::createEmitter("assets/audio/dialogue/hello-big-boy.wav");
//     m_sounds1[1] = idk::AudioSystem::createEmitter("assets/audio/dialogue/seeya-later-big-boy.wav");

//     m_sounds2[0] = idk::AudioSystem::createEmitter("assets/audio/dialogue/i-have-a-gun.wav");


//     world.EM.on("VehicleCanoe::drive", [this]( ActorBase *A ) {
//         if (A == this) { idk::AudioSystem::playSound(m_sounds0[rand()%3], false); }
//     });

//     world.EM.on("VehicleCanoe::undrive", [this]( ActorBase *A ) {
//         if (A == this) { idk::AudioSystem::playSound(m_sounds0[3], false); }
//     });

//     world.EM.on("VehicleTruck::drive", [this]( ActorBase *A ) {
//         if (A == this) { idk::AudioSystem::playSound(m_sounds1[0], false); }
//     });

//     world.EM.on("VehicleTruck::undrive", [this]( ActorBase *A ) {
//         if (A == this) { idk::AudioSystem::playSound(m_sounds1[1], false); }
//     });

//     world.EM.on("WeaponRanged::WeaponRanged", [this]( ActorBase *A ) {
//         if (A == this) { idk::AudioSystem::playSound(m_sounds2[0], false); }
//     });

// }


void
CharacterFreecam::move( const glm::vec3 &delta )
{
    glm::mat3 R = glm::mat3(idk::TransformSys::getModelMatrix(m_attach_obj));
    glm::vec3 dir = (R * delta);

    float l0 = glm::length2(delta);
    float l1 = glm::length2(dir);

    if (l0 > 0.0f && l1 > 0.0f)
    {
        dir = sqrt(l0) * glm::normalize(dir);
        ActorBase::move(delta);
    }
}


void
CharacterFreecam::look( const glm::vec2 &delta )
{
    auto &cmp0 = idk::TransformSys::getTransformCmp(m_root_obj);
    auto &cmp1 = idk::TransformSys::getTransformCmp(m_hinge_obj);

    cmp0.yaw   += delta.x;
    cmp1.pitch += delta.y;
}




void
CharacterPlayerFPS::move( const glm::vec3 &delta )
{
    glm::mat3 R = glm::mat3(idk::TransformSys::getModelMatrix(m_attach_obj));
    glm::vec3 dir = delta * glm::vec3(1.0f, 0.0f, 1.0f);

    auto &pitch = idk::TransformSys::getTransformCmp(m_attach_obj).pitch;
    auto &roll  = idk::TransformSys::getTransformCmp(m_attach_obj).roll;

    roll += 0.005f * glm::sign(delta.x);
    roll = idk::mixRadians(roll, 0.0f, 0.002f);

    // pitch += 0.25f * delta.z;
    // pitch = glm::mix(pitch, 0.0f, 0.025f);


    float l0 = glm::length2(delta);
    float l1 = glm::length2(dir);

    if (l0 > 0.0f && l1 > 0.0f)
    {
        dir = sqrt(l0) * glm::normalize(dir);
        ActorBase::move(dir);
    }
}


void
CharacterPlayerFPS::look( const glm::vec2 &delta )
{
    auto &cmp0 = idk::TransformSys::getTransformCmp(m_root_obj);
    auto &cmp1 = idk::TransformSys::getTransformCmp(m_hinge_obj);

    cmp0.yaw   += delta.x;
    cmp1.pitch += delta.y;


    auto &roll  = idk::TransformSys::getTransformCmp(m_attach_obj).roll;

    roll += 0.25f * delta.x;
    roll = idk::mixRadians(roll, 0.0f, 0.05f);


}

// void
// CharacterPlayerFPS::jump()
// {
//     if (this->isGrounded() == false)
//     {
//         return;
//     }

//     this->setGrounded(false);
//     std::cout << "\nJUMP\n";

//     const float G = idk::PhysicsConstants::G;

//     auto &adesc = getActorDesc();

//     glm::vec3 imp = adesc.jmp_imp * glm::vec3(0.0f, G, 0.0f);
//     glm::vec3 acc = adesc.jmp_acc * glm::vec3(0.0f, G, 0.0f);

//     this->multiplyVel(glm::vec3(1, 0, 1));
//     this->multiplyForce(glm::vec3(1, 0, 1));

//     this->addImpulse(imp);
//     this->addForce(acc);
// }


