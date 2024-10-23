// #include "humanoid.hpp"

// #include <IDKAudio/IDKAudio.hpp>
// #include <IDKBuiltinCS/sys-audio.hpp>
// #include <libidk/idk_math.hpp>

// #include "../systems/sys-grabbable.hpp"
// #include "../systems/sys-weapon.hpp"
// #include "../world.hpp"


// using namespace idk;


// meatworld::Humanoid::Humanoid( const glm::vec3 &position, float yaw )
// :   CharacterBase("humanoid", position, yaw)
// {
//     std::cout << "Humanoid::Humanoid\n";

//     // m_root_obj  = ECS2::createGameObject("humanoid-root", false);
//     m_torso_obj = ECS2::createGameObject("torso", false);
//     m_head_obj  = ECS2::createGameObject("head", false);

//     ECS2::giveChild(m_root_obj, m_torso_obj);
//     ECS2::giveChild(m_torso_obj, m_head_obj);

//     ECS2::giveComponent<TransformCmp>(m_root_obj);
//     // ECS2::giveComponent<KinematicCapsuleCmp>(m_root_obj);
//     // TransformSys::setPositionLocalspace(m_root_obj, position);

//     ECS2::giveComponent<ModelCmp>(m_torso_obj);
//     ECS2::giveComponent<ModelCmp>(m_head_obj);

//     TransformSys::setPositionLocalspace(m_torso_obj, glm::vec3(0.0f));
//     TransformSys::setPositionLocalspace(m_head_obj, glm::vec3(0.0f, 0.45f, 0.0f));

//     TransformSys::getUniformScale(m_torso_obj) = 0.15f;
//     TransformSys::getUniformScale(m_head_obj)  = 0.2f;

//     ModelSys::assignModel(m_torso_obj, "assets/models/npc/meat-torso.idkvi");
//     ModelSys::assignModel(m_head_obj,  "assets/models/npc/meat-head.idkvi");



//     m_hit_obj = ECS2::createGameObject("hitbox", false);
//     ECS2::giveComponent<CharacterHitBoxCmp>(m_hit_obj);
//     TransformSys::getLocalPosition(m_hit_obj) = glm::vec3(0.0f, -0.11f, 0.08f);
//     TransformSys::getXYZScale(m_hit_obj) = glm::vec3(0.5f, 0.481f, 0.33f);

//     auto &cmp = ECS2::getComponent<CharacterHitBoxCmp>(m_hit_obj);
//     cmp.callback = [this]()
//     {
//         m_health -= 1;
//         std::cout << "Health: " << m_health << "\n";
//     };


//     ECS2::giveChild(m_root_obj, m_hit_obj);


//     {
//         glm::vec3 l_offset = glm::vec3(-0.15f, +0.3f, 0.0f);
//         glm::vec3 r_offset = glm::vec3(+0.15f, +0.3f, 0.0f);

//         glm::vec3 l_rest = glm::vec3(0.05f, -0.75f, -0.01f);
//         glm::vec3 r_rest = glm::vec3(0.05f, -0.75f, -0.01f);

//         m_arms[0] = new ProcArmTest(m_root_obj, l_offset, l_rest);
//         m_arms[1] = new ProcArmTest(m_root_obj, r_offset, r_rest);
//     }


//     {
//         ProcLegDesc desc = {
//             .joints      = 3,
//             .dists       = {0.35f, 0.45f},
//             .min_stride  = glm::vec3(0.05f, 0.0f, 0.05f),
//             .max_stride  = glm::vec3(0.1f,  0.0f, 0.35f),
//             .step_height = glm::vec3(0.15f, 0.0f, 0.25f),
//             .step_speed  = glm::vec3(1.0f, 0.0f, 0.25f),
//             .speed       = 1.5f
//         };

//         m_leg_controller.createLeg(desc);
//         m_leg_controller.createLeg(desc);

//         auto &leg0 = m_leg_controller.getLeg(0);
//         auto &leg1 = m_leg_controller.getLeg(1);

//         leg0.setOffset(glm::vec3(-0.08f, -0.1f, 0.0f));
//         leg1.setOffset(glm::vec3(+0.08f, -0.1f, 0.0f));
//         leg1.m_xanim.advance(3.14159f);
//         leg1.m_zanim.advance(3.14159f);
//     }
// }


// meatworld::Humanoid::~Humanoid()
// {
//     // std::cout << "Humanoid::~Humanoid, weapon = " << m_weapon << "\n";

//     if (m_weapon)
//     {
//         delete m_weapon;
//         m_weapon = nullptr;
//     }

//     delete m_arms[0];
//     delete m_arms[1];

//     ECS2::deleteGameObject(m_root_obj);
// }



// void
// meatworld::Humanoid::update( idk::EngineAPI &api )
// {
//     auto &engine = api.getEngine();
//     auto &ren    = api.getRenderer();
//     auto &events = api.getEventSys();
//     float dt = engine.deltaTime();

//     m_leg_controller.draw(api);

//     m_arms[0]->update(dt);
//     m_arms[1]->update(dt);

//     for (int i=0; i<2; i++)
//     {
//         m_arms[i]->draw(api);
//     }
// }


// void
// meatworld::Humanoid::move( idk::EngineAPI &api, const CharacterMotion &motion )
// {
//     auto &engine = api.getEngine();
//     auto &ren    = api.getRenderer();
//     float dt     = engine.deltaTime();

//     glm::mat4 T = TransformSys::getModelMatrix(m_root_obj);

//     glm::vec3 velocity  = motion.delta;
//               velocity *= (motion.run) ? m_run_speed : m_walk_speed;

//     glm::vec3 delta = dt * velocity;


//     if (m_weapon)
//     {
//         m_weapon->update(api, 0.0f, 0.0f);

//         if (motion.aim && motion.attack)
//         {
//             float recoil = m_weapon->attack(api);
//         }
//     }


//     {
//         float &A = TransformSys::getTransformCmp(m_root_obj).yaw;
//         float  B = motion.yaw;

//         float alpha = 0.0f;

//         if (motion.aim)
//         {
//             alpha = 0.25f;
//         }

//         else if (glm::length2(motion.delta) > 0.0f)
//         {
//             alpha = 0.05f;
//         }

//         A = idk::mixRadians(A, motion.yaw, alpha);
//     }



//     glm::vec3 &pos = TransformSys::getLocalPosition(m_root_obj);
//     pos += glm::mat3(T) * m_leg_controller.update(velocity, dt, T);

//     glm::vec3  foot_avg  = m_leg_controller.getAvgFootPosition();
//     glm::vec3  foot_min  = m_leg_controller.getMinFootPosition();


//     float dy = foot_min.y + 0.8f;

//     if (motion.crouch)
//     {
//         dy = foot_avg.y + 0.5f;

//         float &pitch = TransformSys::getTransformCmp(m_root_obj).pitch;
//         if (pitch > glm::radians(-30.0f))
//         {
//             pitch -= glm::radians(1.0f);
//         }
//     }

//     else
//     {
//         float &pitch = TransformSys::getTransformCmp(m_root_obj).pitch;
//         if (pitch < 0.0f)
//         {
//             pitch += glm::radians(1.0f);
//         }
//     }

//     pos.y = glm::mix(pos.y, dy, 0.1f);

// }


// void
// meatworld::Humanoid::reachFor( const glm::vec3 &target )
// {
//     m_arms[1]->reachFor(target);
// }


