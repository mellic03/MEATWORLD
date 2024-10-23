// #include "./proc-arm.hpp"

// #include <IDKECS/IDKECS.hpp>

// #include <IDKBuiltinCS/IDKBuiltinCS.hpp>
// #include <IDKBuiltinCS/sys-transform.hpp>
// #include <IDKBuiltinCS/sys-physics.hpp>
// #include <IDKBuiltinCS/sys-model.hpp>
// #include <IDKBuiltinCS/sys-particle.hpp>

// #include <libidk/idk_transformIK.hpp>
// #include <libidk/idk_math.hpp>

// #include "../world.hpp"


// using namespace idk;


// meatworld::ProcArmTest::ProcArmTest()
// {
//     m_obj_id = ECS2::createGameObject("limb-end", false);
// }


// meatworld::ProcArmTest::ProcArmTest( int parent, glm::vec3 offset, glm::vec3 rest )
// :   m_parent(parent),
//     m_root_offset(offset),
//     m_rest_offset(offset+rest),
//     m_joints(3),
//     m_dists(2)
// {
//     m_obj_id = ECS2::createGameObject("limb-end", false);
//     ECS2::giveChild(parent, m_obj_id);

//     for (int i=0; i<3; i++)
//     {
//         float x = (rand() % 1000) / 1000.0f;
//         float y = (rand() % 1000) / 1000.0f;
//         float z = (rand() % 1000) / 1000.0f;
    
//         m_joints[i] = float(i) * glm::vec3(x, y, z);
//     }


//     if (offset.x > 0.0f)
//     {
//         m_anim.advance(idk::PI);
//     }


//     m_length   = 0.8f * glm::length(rest);
//     m_dists[0] = 0.4f * m_length;
//     m_dists[1] = 0.6f * m_length;
// }


// meatworld::ProcArmTest::~ProcArmTest()
// {
//     ECS2::deleteGameObject(m_obj_id);
// }




// void
// meatworld::ProcArmTest::_update( float dt )
// {
// }



// void
// meatworld::ProcArmTest::update( float dt )
// {
//     if (ECS2::gameObjectExists(m_obj_id) == false)
//     {
//         m_obj_id = ECS2::createGameObject("limb-end", false);
//     }

//     glm::mat4 T     = TransformSys::getModelMatrix(m_parent);
//     glm::mat3 R     = glm::mat3(T);
//     glm::vec3 front = TransformSys::getFront(m_parent);

//     glm::vec3 target  = m_rest_offset + m_anim.get(0.5, 0.25f, 0.25f);
//               target += m_reach_offset;

//     m_reach_offset *= 0.95f; // pow(0.1f, dt);


//     m_joints[0]  = m_root_offset;
//     m_joints[1] -= 0.05f * m_length * glm::vec3(0.0f, 0.0f, -1.0f);
//     m_joints[1] -= 0.05f * m_length * glm::vec3(0.0f, 1.0f, 0.0f);
//     m_joints[2]  = target;

//     idk::IK::FABRIK(m_joints, m_dists, m_length, 2);

//     TransformSys::getLocalPosition(m_obj_id) = m_joints.back();
// }



// void
// meatworld::ProcArmTest::draw( idk::EngineAPI &api )
// {
//     auto &ren = api.getRenderer();
//     static int model = ren.loadModel("assets/models/npc/meat-arm-u.idkvi");

//     float w = 0.0125f * float(m_joints.size());

//     glm::mat4 M = TransformSys::getModelMatrix(m_parent);


//     for (int i=0; i<m_joints.size() - 1; i++)
//     {
//         glm::vec3 A = glm::vec3(M * glm::vec4(m_joints[i], 1.0f));
//         glm::vec3 B = glm::vec3(M * glm::vec4(m_joints[i+1], 1.0f));

//         ren.drawCapsule(A, B, 0.02f);
        
//         // glm::mat4 R = glm::lookAt(m_joints[i], m_joints[i+1], glm::vec3(0.0f, 1.0f, 0.0f));
//         //           R = glm::mat4(glm::inverse(glm::mat3(R)));

//         // glm::mat4 T  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, m_dists[i]));
//         // glm::mat4 S  = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, glm::length(m_joints[i+1] - m_joints[i])));

//         // ren.drawModel(model, T*R*S);
//         // ren.drawShadowCaster(model, T*R*S);
//     }


// }
