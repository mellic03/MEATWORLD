#include "./limb.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <libidk/idk_transformIK.hpp>
#include <libidk/idk_math.hpp>

#include "../world.hpp"


using namespace idk;


meatworld::Limb::Limb( int joints, const std::vector<float> &distances )
:   m_joints (joints),
    m_dists  (distances)
{
    for (int i=0; i<joints; i++)
    {
        float x = (rand() % 1000) / 1000.0f;
        float y = (rand() % 1000) / 1000.0f;
        float z = (rand() % 1000) / 1000.0f;
    
        m_joints[i] = float(i) * glm::vec3(x, y, z);
    }

    m_total_dist = 0.0f;
    for (float d: m_dists)
    {
        m_total_dist += d;
    }

}



void
meatworld::Limb::setOffset( const glm::vec3 &offset )
{
    m_offset = offset;
}



void
meatworld::Limb::_update( const glm::vec3 &start, const glm::vec3 &end )
{
    m_joints.front() = start;
    m_joints.back()  = end;

    idk::IK::FABRIK(m_joints, m_dists, m_total_dist, 1);
}


void
meatworld::Limb::draw( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    static int model = ren.loadModel("assets/models/npc/meat-arm-u.idkvi");

    float w = 0.0125f * float(m_joints.size());

    for (int i=0; i<m_joints.size() - 1; i++)
    {
        glm::mat4 TR = glm::inverse(glm::lookAt(m_joints[i+1], m_joints[i], glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::mat4 T  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_dists[i]));
        glm::mat4 S  = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, glm::length(m_joints[i+1] - m_joints[i])));

        ren.drawModel(model, TR*T*S);
        ren.drawShadowCaster(model, TR*T*S);
    }

}




void
meatworld::ProcArm::update( const glm::vec3 &velocity, float dt, const glm::mat4 &T,
                            const glm::vec3 &aim )
{
    glm::vec3 motion = dt * velocity;

    m_anim.advance(8.0f*dt);

    if (glm::length2(motion) > 0.0f)
    {
        m_potential += glm::length(motion);
    }

    else
    {
        m_potential -= dt;
    }
    m_potential = glm::clamp(m_potential, 0.0f, 1.0f);


    float xsign = glm::sign(m_offset.x);

    glm::vec3 front = glm::mat3(T) * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 start = glm::vec3(T[3]) + glm::mat3(T) * m_offset;
    glm::vec3 end   = glm::vec3(T[3]) + glm::mat3(T) * glm::vec3(xsign*0.25f, -0.1f, -0.1f);

    if (aim != glm::vec3(0.0f))
    {
        end = start + glm::vec3(0.25f, 0.0f, 0.0f) + 0.95f * m_total_dist * aim;
        m_joints[1] -= glm::vec3(0.0f, 0.5f, 0.0f);
    }

    else
    {
        end += m_potential * 0.25f * glm::mat3(T) * m_anim.get(0.5f, 0.125f, 0.25f);

        m_joints[1] -= front;
    }

    end = glm::mix(m_joints.back(), end, 0.1f);

    _update(start, end);
}




glm::vec3
meatworld::ProcLeg::update( const glm::vec3 &velocity, float dt, const glm::mat4 &T )
{
    glm::mat3 R      = glm::mat3(T);
    glm::mat3 invR   = glm::inverse(R);
    glm::vec3 motion = dt * velocity;

    glm::vec3 stride = glm::mix(m_desc.min_stride, m_desc.max_stride, m_momentum);

    float H  = m_desc.step_height[2];
    float XS = stride.x;
    float ZS = stride.z;


    if (glm::sign(m_offset.x) == glm::sign(motion.x))
    {
        XS = stride.z;
    }


    if (glm::length2(velocity) > 0.0f)
    {
        m_motion = motion;
        m_momentum += 4.0f*dt;
    }

    else
    {
        m_momentum -= 8.0f*dt;
    }

    m_momentum = glm::clamp(m_momentum, 0.0f, 1.0f);
    // idk_printvalue(m_momentum);


    glm::vec3 sp = m_desc.step_speed * m_motion;

    glm::vec3 xsp = sp * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 zsp = sp * glm::vec3(0.0f, 0.0f, 1.0f);

    m_xanim.advance(0.02f * (m_desc.speed + glm::dot(xsp, xsp)) * m_momentum);


    glm::vec3 front = R * glm::vec3(0.0f, 0.0f, -1.0f);
    m_joints[1] += 0.05f * front;

    glm::vec3 origin = glm::vec3(T[3]) + (R * m_offset);
    glm::vec3 dir    = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 hit;


    if (GameWorld::raycastDown(origin-0.1f*front, &hit))
    {
        if (glm::distance(origin, hit) < 1.5f * m_total_dist)
        {
            glm::vec3 local_motion = m_xanim.get(H, XS, ZS);
            glm::vec3 world_motion = R * (local_motion * glm::sign(m_motion));

            glm::vec3 end = hit + m_momentum*world_motion;
                      end.y += m_momentum*local_motion.y;

            _update(origin, end);
        }

        else
        {
            _update(origin, origin - glm::vec3(0.0f, m_total_dist, 0.0f));
        }
    }

    m_height = m_joints.back().y;


    glm::vec3 rmotion      = m_xanim.rootmotion(H, XS, ZS);
    glm::vec3 local_motion = m_momentum * rmotion * glm::sign(m_motion);

    if (glm::dot(glm::normalize(local_motion), glm::normalize(m_motion)) < 0.0f)
    {
        return -local_motion;
    }

    return glm::vec3(0.0f);
}


// void
// meatworld::ProcLeg::update( const glm::vec3 &velocity, float dt, const glm::mat4 &T )
// {
//     glm::mat3 R      = glm::mat3(T);
//     glm::vec3 motion = dt * velocity;
//     glm::vec3 stride = glm::mix(m_desc.min_stride, m_desc.max_stride, m_potential);

//     const float H  = 0.5f;
//     const float XS = stride.x;
//     const float ZS = stride.z;

//     if (glm::length2(motion) > 0.0f)
//     {
//         m_potential += glm::length(motion);
//         m_motion = glm::mix(m_motion, motion, 0.5f);
//     }

//     else
//     {
//         m_potential -= dt;
//         m_motion *= m_potential;
//     }

//     m_potential = glm::clamp(m_potential, 0.0f, 1.0f);

//     glm::vec3 origin = glm::vec3(T[3]) + (R * m_offset);
//     glm::vec3 dir    = glm::vec3(0.0f, -1.0f, 0.0f);
//     glm::vec3 hit;

//     m_joints[1] += R * glm::vec3(0.0f, 0.0f, -0.05f);

//     if (GameWorld::raycastDown(origin, &hit))
//     {
//         glm::vec3 diff = R * (m_curr_anchor - hit);

//         float dx = fabsf(diff.x);
//         float dz = fabsf(diff.z);

//         if (dx >= stride.x || dz >= stride.z)
//         {
//             m_prev_anchor = m_curr_anchor;
//             m_curr_anchor = hit;

//             m_timer = 0.0f;
//         }
//     }


//     float alpha = glm::clamp(m_timer / m_desc.step_speed, 0.0f, 1.0f);
//     m_timer += dt;

//     if (alpha < 1.0f)
//     {
//         m_target    = glm::mix(m_prev_anchor, m_curr_anchor, alpha);
//         m_target.y += m_desc.step_height[2] * sinf(alpha * idk::PI);
//     }

//     else
//     {
//         m_prev_anchor = m_curr_anchor;
//     }


//     _update(origin, m_target);

//     m_height = m_joints.back().y;

// }



int
meatworld::ProcLegController::createLeg( const ProcLegDesc &desc )
{
    m_legs.push_back(ProcLeg(desc));
    const size_t W = m_legs.size();

    m_states.resize(W);
    m_adjacency.resize(W);

    for (int i=0; i<W; i++)
    {
        m_states[i] = 1;
        m_adjacency[i] = -1;
    }

    return W - 1;
}



void
meatworld::ProcLegController::setAdjacency( int a, int b, bool adjacency )
{
    m_adjacency[a] = b;
    m_adjacency[b] = a;
}




glm::vec3
meatworld::ProcLegController::update( const glm::vec3 &motion, float dt, const glm::mat4 &T )
{
    const size_t W = m_legs.size();

    glm::vec3 center = glm::vec3(T[3]) * glm::vec3(1.0f, 0.0f, 1.0f);
    glm::vec3 dir    = glm::normalize(glm::mat3(T) * (motion/dt));

    glm::vec3 result = glm::vec3(0.0f);

    for (int i=0; i<W; i++)
    {
        ProcLeg &leg = m_legs[i];

        result += leg.update(motion, dt, T);
    }

    return result;
}



void
meatworld::ProcLegController::draw( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    for (auto &leg: m_legs)
    {
        leg.draw(api);
    }
}


glm::vec3
meatworld::ProcLegController::getAvgFootPosition()
{
    glm::vec3 avg = glm::vec3(0.0f);

    for (auto &leg: m_legs)
    {
        avg += leg.getJoints().back();
    }

    return avg / float(m_legs.size());
}


glm::vec3
meatworld::ProcLegController::getMinFootPosition()
{
    glm::vec3 avg = glm::vec3(INFINITY);

    for (auto &leg: m_legs)
    {
        avg.y = glm::min(avg.y, leg.m_height);
    }

    return avg;
}
