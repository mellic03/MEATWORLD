#pragma once

#include <vector>
#include <libidk/idk_glm.hpp>
#include "animation.hpp"
#include "character.hpp"


namespace idk
{
    class EngineAPI;
}

namespace meatworld
{
    class  ProcArmTest;
}



class meatworld::ProcArmTest
{
protected:

    int       m_parent = -1;
    int       m_obj_id = -1;
    float     m_length = 1.0f;

    glm::vec3 m_root_offset  = glm::vec3(0.0f);
    glm::vec3 m_rest_offset  = glm::vec3(0.0f);
    glm::vec3 m_reach_offset = glm::vec3(0.0f);

    std::vector<glm::vec3> m_joints;
    std::vector<float>     m_dists;

    LegWalkAnimation m_anim;


public:

    ProcArmTest();
    ProcArmTest( int parent, glm::vec3 offset, glm::vec3 rest );
    ~ProcArmTest();


    void _update( float dt );
    void update( float dt );
    void draw( idk::EngineAPI& );

    void reachFor( const glm::vec3 &target )
    {
        glm::vec3 dir = -glm::vec3(0.0f, 0.0f, m_length) - (-m_root_offset + m_rest_offset);

        m_reach_offset += 0.1f * dir;

        // if (glm::length2(m_reach_offset) > m_length*m_length)
        // {
        //     m_reach_offset = 0.9f * m_length * glm::normalize(m_reach_offset);
        // }
    }

    int objID() const { return m_obj_id; };
};



