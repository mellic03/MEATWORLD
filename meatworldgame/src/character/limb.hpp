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
    class Limb;

    struct ProcArmDesc;
    struct ProcLegDesc;

    class  ProcArm;
    class  ProcLeg;
    class  ProcLegController;
}



class meatworld::Limb
{
protected:
    glm::vec3 m_offset = glm::vec3(0.0f);
    std::vector<glm::vec3> m_joints;
    std::vector<float>     m_dists;
    float m_total_dist = 1.0f;


public:

    Limb( int joints, const std::vector<float>&  );
    void _update( const glm::vec3 &start, const glm::vec3 &end );

    void setOffset( const glm::vec3& );
    glm::vec3 getOffset() { return m_offset; };
    const auto &getJoints() { return m_joints; };
    void draw( idk::EngineAPI& );
};




struct meatworld::ProcArmDesc
{
    int joints = 3;
    std::vector<float> dists = { 0.5f, 0.65f };

    // glm::vec3 min_stride  = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 max_stride  = glm::vec3(0.1f, 0.0f, 0.25f);
    // glm::vec3 step_height = glm::vec3(0.25f, 0.0f, 0.65f);

    // float step_speed  = 1.0f;
    // float max_speed   = 1.0f;
};



class meatworld::ProcArm: public meatworld::Limb
{
private:


public:
    LegWalkAnimation m_anim;
    float m_potential = 0.0f;

    ProcArm( const ProcArmDesc &desc )
    :   Limb(desc.joints, desc.dists)
    {

    }

    void update( const glm::vec3&, float dt, const glm::mat4 &T, const glm::vec3 &aim = glm::vec3(0.0f) );

};





struct meatworld::ProcLegDesc
{
    int joints = 3;
    std::vector<float> dists = { 0.5f, 0.65f };

    glm::vec3 min_stride  = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 max_stride  = glm::vec3(0.1f, 0.0f, 0.25f);
    glm::vec3 step_height = glm::vec3(0.25f, 0.0f, 0.65f);
    glm::vec3 step_speed  = glm::vec3(2.0f,  0.0f, 2.0f);

    float speed = 1.0f;
};


class meatworld::ProcLeg: public meatworld::Limb
{
private:
    glm::vec3 m_heading = glm::vec3(0.0f);
    float     m_speed   = 0.0f;

    glm::vec3 m_target = glm::vec3(0.0f);

    glm::vec3 m_prev_anchor = glm::vec3(0.0f);
    glm::vec3 m_curr_anchor = glm::vec3(0.0f);
    glm::vec3 m_next_anchor = glm::vec3(0.0f);

    glm::vec3 m_motion = glm::vec3(0.0f);
    float     m_momentum = 0.0f;


public:
    ProcLegDesc m_desc;
    LegWalkAnimation m_xanim;
    LegWalkAnimation m_zanim;

    float m_height = 0.0f;
    float m_accum  = 0.0f;


    ProcLeg( const ProcLegDesc &desc )
    :   Limb   (desc.joints, desc.dists),
        m_desc (desc)
    {
        
    }
    
    
    glm::vec3 update( const glm::vec3&, float dt, const glm::mat4 &T );

};



class meatworld::ProcLegController
{
private:
    std::vector<int>     m_states;
    std::vector<int>     m_adjacency;


public:
    std::vector<ProcLeg> m_legs;

    int createLeg( const ProcLegDesc& );
    ProcLeg &getLeg( int id ) { return m_legs[id]; };
    void setAdjacency( int a, int b, bool adjacency );

    glm::vec3 getAvgFootPosition();
    glm::vec3 getMinFootPosition();

    glm::vec3 update( const glm::vec3&, float dt, const glm::mat4 &T );
    void draw( idk::EngineAPI& );

};

