// #include "player.hpp"
// // #include "behaviour.hpp"
// #include "flashlight.hpp"
// #include "weapon.hpp"
// #include "vehicle.hpp"
// #include "../world/world.hpp"

// #include <IDKBuiltinCS/IDKBuiltinCS.hpp>
// #include <IDKEvents/IDKEvents.hpp>
// #include <IDKGameEngine/IDKGameEngine.hpp>

// using namespace meat;



// void
// ControllerPlayer::update( idk::EngineAPI &api, ActorBase *C )
// {
//     float dt     = api.dtime();
//     auto &events = api.getEventSys();
//     auto &k      = events.keylog();

//     glm::vec3 kd = glm::vec3(0.0f);
//     float speed = C->getWalkSpeed();

//     if (k.keyDown(idk::Keycode::A)) kd.x -= speed * dt;
//     if (k.keyDown(idk::Keycode::D)) kd.x += speed * dt;
//     if (k.keyDown(idk::Keycode::W)) kd.z -= speed * dt;
//     if (k.keyDown(idk::Keycode::S)) kd.z += speed * dt;

//     C->move(api, kd);

//     if (events.mouseCaptured())
//     {
//         glm::vec2 md = glm::vec2(-1) * 0.0005f * events.mouseDelta();
//         C->look(md);
//     }
// }



// CharacterFreecam::CharacterFreecam( World &world, const glm::vec3 &pos, float yaw )
// :   ActorBase(world, "Freecam", pos, yaw),
//     m_cam_obj(m_attach_obj)
// {
//     m_hinge_obj = idk::ECS2::createGameObject("Hinge", false);

//     idk::ECS2::removeChild(m_root_obj, m_attach_obj);
//     idk::ECS2::giveChild(m_root_obj, m_hinge_obj);
//     idk::ECS2::giveChild(m_hinge_obj, m_attach_obj);

//     idk::TransformSys::getLocalPosition(m_hinge_obj) *= 0.0f;
//     idk::TransformSys::getLocalPosition(m_attach_obj) *= 0.0f;

//     setActorDesc({0.0f, 16.0f, 32.0f});

//     idk::ECS2::setGameObjectName(m_cam_obj, "Camera");
//     idk::ECS2::giveComponent<idk::CameraCmp>(m_cam_obj);
//     idk::ECS2::giveComponent<idk::AudioListenerCmp>(m_cam_obj);

//     // giveBehaviour<BehaviourPlayerKeyInput>();
//     // giveBehaviour<BehaviourPlayerMouseInput>();
// }


// void
// CharacterFreecam::move( idk::EngineAPI &api, const glm::vec3 &delta )
// {
//     glm::mat3 R = glm::mat3(idk::TransformSys::getModelMatrix(m_cam_obj));
//     idk::TransformSys::getLocalPosition(m_root_obj) += R * delta;
// }


// void
// CharacterFreecam::look( const glm::vec2 &delta )
// {
//     auto &cmp0 = idk::TransformSys::getTransformCmp(m_root_obj);
//     auto &cmp1 = idk::TransformSys::getTransformCmp(m_hinge_obj);

//     cmp0.yaw   += delta.x;
//     cmp1.pitch += delta.y;
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


// void
// CharacterPlayerFPS::move( idk::EngineAPI &api, const glm::vec3 &delta )
// {
//     glm::mat3 R = glm::mat3(idk::TransformSys::getModelMatrix(m_cam_obj));
//     glm::vec3 dir = R * (delta * glm::vec3(1.0f, 0.0f, 1.0f));

//     float l0 = glm::length2(delta);
//     float l1 = glm::length2(dir);

//     if (l0 > 0.0f && l1 > 0.0f)
//     {
//         dir = sqrt(l0) * glm::normalize(dir);
//         this->translateWorld(dir);
//     }
// }


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



