#pragma once

#include <vector>
#include <glm/glm.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
    #define GLM_ENABLE_EXPERIMENTAL
    #include <glm/gtx/quaternion.hpp>
#endif


namespace idk { class RenderEngine; }


namespace meat
{
    class PhysicsBody;

    class RigidPoint;
    class RigidBody;

    glm::vec3 compute_torque( const glm::vec3 &origin, const glm::vec3 effector,
                              const glm::vec3 &force );

}




class meat::PhysicsBody
{
private:
    PhysicsBody *m_parent = nullptr;

    glm::mat4 m_transform[2] = { glm::mat4(1.0f), glm::mat4(1.0f) };

    float     m_accum = 0.0f;
    float     m_mass  = 1.0f;

    glm::vec3 m_pos;
    glm::quat m_rot = glm::quat(glm::vec3(0.0f));
    glm::vec3 m_sc  = glm::vec3(1.0f);

    glm::vec3 m_vel = glm::vec3(0.0f);

    glm::vec3 m_linacc = glm::vec3(0.0f);
    glm::vec3 m_linvel = glm::vec3(0.0f);

    glm::vec4 m_angacc = glm::vec4(0.0f);
    glm::vec3 m_angvel = glm::vec3(0.0f);


public:
    PhysicsBody( const glm::vec3 &p, const glm::vec3 &v = glm::vec3(0.0f) )
    :   m_pos(p), m_linvel(v)
    {

    }

    PhysicsBody( PhysicsBody *parent, const glm::vec3 &p, const glm::vec3 &v = glm::vec3(0.0f) )
    :   m_parent(parent), m_pos(p), m_linvel(v)
    {

    }

    virtual void update( float dt );

    void applyScale   ( const glm::vec3 &sc ) { m_sc = sc; }
    void applyLinear  ( const glm::vec3& );
    void applyAngular ( const glm::vec3& );

    void setMass( float m ) { m_mass = m; }


    const glm::vec3 getLocalPosition() const { return m_pos;    } 
    const glm::vec3 getLocalVelocity() const { return m_linvel; }


    const PhysicsBody *getParent()    const { return m_parent; }
    const glm::vec3    getPosition()  const { return glm::vec3(m_transform[0][3]); } 
    const glm::vec3   &getVelocity()  const { return m_vel; }
    const glm::mat4   &getTransform() const { return m_transform[0]; }
};





class meat::RigidPoint: public PhysicsBody
{
private:
    glm::vec3 m_prev_pos = glm::vec3(0.0f);

public:
    RigidPoint( PhysicsBody *parent, const glm::vec3 &offset )
    :   PhysicsBody(parent, offset)
    {
        setMass(0.0f);
    }

    virtual void update( float dt ) override { PhysicsBody::update(dt); };
    void update( float dt, glm::vec3 &linear, glm::vec3 &angular );

};



class meat::RigidBody: public PhysicsBody
{
private:
    std::vector<RigidPoint> m_points;

public:
    RigidBody( const glm::vec3&, const glm::vec3 &v = glm::vec3(0.0f) );

    virtual void update( float dt );
    void addRigidPoint( const glm::vec3& );

    void drawPoints( idk::RenderEngine& );

};



