#include "rigidbody.hpp"
#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKGraphics/IDKGraphics.hpp>

using namespace meat;



// glm::vec3 F = m_thrust * -front;
// glm::vec3 r = (ppos - wpos);
// glm::vec3 torque = glm::cross(r, F);

// glm::vec3 mo(1.0f);
// glm::vec3 angular = api.dtime() * (torque / (mo * mass));
// glm::vec3 linear  = api.dtime() * (m_thrust / (mo * mass)) * -front;

// glm::vec3 axis  = glm::normalize(angular);
// float     angle = glm::radians(glm::length(angular));

// idk::TransformSys::rotateWorldAxis(obj, axis, angle);


glm::vec3
meat::compute_torque( const glm::vec3 &origin, const glm::vec3 effector, const glm::vec3 &force )
{
    glm::vec3 F   = force;
    glm::vec3 r   = effector - origin;
    glm::vec3 tau = glm::cross(r, F);

    if (glm::length2(tau) < 0.005f)
    {
        return glm::vec3(0.0f);
    }

    return tau;
}



void
PhysicsBody::update( float dt )
{
    static const float tstep = 1.0f / 30.0f;
    m_accum += dt;

    while (m_accum >= tstep)
    {
        m_linvel *= 0.999f;
        // m_angvel *= 0.992f;
        m_accum  -= tstep;
    }

    glm::vec3 G = glm::vec3(0.0f, -1.8f, 0.0f);

    if (m_parent)
    {
        G = m_mass*G;
    }


    m_pos    += dt*m_linvel + 0.5f * dt*dt*(m_linacc + G);
    m_linvel += dt*(m_linacc + G);

    if (m_angvel != glm::vec3(0.0f) && glm::length2(m_angvel) >= 0.01f)
    {
        auto  axis  = glm::normalize(m_angvel);
        float angle = glm::radians(glm::length(m_angvel));

        glm::quat q = glm::angleAxis(dt*angle, axis);
        m_rot = glm::normalize(q * m_rot);
    }

    glm::mat4 T = glm::translate(glm::mat4(1.0f), m_pos);
    glm::mat4 R = glm::mat4_cast(m_rot);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), m_sc);

    m_transform[1] = m_transform[0];

    if (m_parent)
    {
        m_transform[0] = m_parent->getTransform() * T*R*S;
    }

    else
    {
        m_transform[0] = T*R*S;
    }

    m_vel = glm::vec3(m_transform[0][3] - m_transform[1][3]);
}


void
PhysicsBody::applyLinear( const glm::vec3 &linear )
{
    glm::mat4 T = (glm::translate(glm::mat4(1), m_pos) * glm::mat4_cast(m_rot));
    m_linvel += glm::vec3(T * glm::vec4(linear, 0.0f));
}


void
PhysicsBody::applyAngular( const glm::vec3 &angular )
{
    glm::mat4 T = (glm::translate(glm::mat4(1), m_pos) * glm::mat4_cast(m_rot));
    m_angvel += glm::vec3(T * glm::vec4(angular, 0.0f));
}






RigidBody::RigidBody( const glm::vec3 &pos, const glm::vec3 &vel)
:   PhysicsBody(pos, vel)
{
    addRigidPoint(0.5f * glm::vec3(-1, -1, -1));
    addRigidPoint(0.5f * glm::vec3(-1, -1, +1));
    addRigidPoint(0.5f * glm::vec3(-1, +1, -1));
    addRigidPoint(0.5f * glm::vec3(-1, +1, +1));
    addRigidPoint(0.5f * glm::vec3(+1, -1, -1));
    addRigidPoint(0.5f * glm::vec3(+1, -1, +1));
    addRigidPoint(0.5f * glm::vec3(+1, +1, -1));
    addRigidPoint(0.5f * glm::vec3(+1, +1, +1));

    applyScale(glm::vec3(2.0f, 2.0f, 2.0f));
}



void
RigidBody::update( float dt )
{
    PhysicsBody::update(dt);

    for (auto &p: m_points)
    {
        p.update(dt);
    }


    glm::vec3 linear  = glm::vec3(0.0f);
    glm::vec3 angular = glm::vec4(0.0f);

    for (auto &p: m_points)
    {
        p.update(dt, linear, angular);

        applyLinear(linear);
        applyAngular(angular);
    }


    static const glm::vec3 G = glm::vec3(0.0f, -1.8f, 0.0f);
    auto pos = getPosition();

    for (auto &p: m_points)
    {
        angular = compute_torque(pos, p.getPosition(), G/8.0f);
        applyAngular(angular);
    }

}



void
RigidBody::drawPoints( idk::RenderEngine &ren )
{
    for (auto &p: m_points)
    {
        auto T = p.getTransform();
        ren.drawSphere(glm::vec3(T[3]), 0.125f);
    }

}




void
RigidPoint::update( float dt, glm::vec3 &linear, glm::vec3 &angular )
{
    glm::vec3 parent_wspace = getParent()->getPosition();
    glm::vec3 pos_wspace    = getPosition();

    glm::vec4 result = glm::vec4(0.0f);
    float h = idk::TerrainRenderer::heightQuery(pos_wspace.x, pos_wspace.z);

    linear  = glm::vec3(0.0f);
    angular = glm::vec3(0.0f);


    float overlap = h - pos_wspace.y;

    if (overlap > 0.0f)
    {
        glm::vec3 N = idk::TerrainRenderer::slopeQuery(pos_wspace.x, pos_wspace.z);
        glm::vec3 F = overlap * N;

        linear   = F;
        linear  -= 0.5f * getVelocity() * glm::vec3(1, 0, 1);

        angular  = compute_torque(parent_wspace, pos_wspace, F);
        angular += compute_torque(parent_wspace, pos_wspace, getVelocity());
    }
}


void
RigidBody::addRigidPoint( const glm::vec3 &offset )
{
    m_points.push_back(RigidPoint(this, offset));
}