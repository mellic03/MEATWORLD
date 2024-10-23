#pragma once

#include "../character/character.hpp"
// #include "behaviour.hpp"


namespace idk
{
    class EngineAPI;
}


namespace meat
{
    struct VehicleDesc
    {
        glm::vec3   seat_pos    = glm::vec3(0.0f, 0.0f, -1.5f);
        glm::vec3   model_pos   = glm::vec3(0.0f);
        glm::vec3   model_scale = glm::vec3(1.0f);
        std::string model_path  = "assets/models/unit-cube.idkvi";
        float       acc         = 8.0f;
        float       steer_spd   = 4.0f;
        float       steer_damp  = 2.0f;
        float       track       = 2.0f; // widthwise dist between wheels
        float       wheelbase   = 3.0f; // lengthwise dist between wheels
        float       suspension  = 1.0f;
        glm::vec3   spring      = glm::vec3(0.25f, 1.0f, 1.0f); //
    };

    class VehicleBase;
    class VehicleWater;
    class VehicleLand;
    class VehicleAir;

    class VehicleCanoe;
    class VehicleShipPinnacle;
    class VehicleTruck;

    class BehaviourOrientVehicle;
    class BehaviourOrientVehicleWater;

    class BehaviourDriveVehicle;
}


class meat::VehicleBase: public ActorBase
{
private:
    float m_steering = 0.0f;
    float m_speed    = 0.0f;

    VehicleDesc m_desc;
    ActorBase *m_driver;

protected:
    void setVehicleDesc( const VehicleDesc &desc );

public:
    VehicleBase( World &world,
                 const std::string &name = "VehicleBase",
                 const glm::vec3 &pos=glm::vec3(0.0f) );

    virtual void update( idk::EngineAPI&, meat::World& ) override;

    void drive( ActorBase* );
    void undrive();
    const ActorBase *getDriver() const { return m_driver; }

    VehicleDesc &getVehicleDesc() { return m_desc; }
    virtual const glm::vec3 &getSeatPosition() const { return m_desc.seat_pos; }

    virtual void move( const glm::vec3& ) override;
};




class meat::VehicleWater: public VehicleBase
{
public:
    VehicleWater( World &world,
                  const std::string &name = "VehicleWater",
                  const glm::vec3 &pos=glm::vec3(0.0f) );

};


class meat::VehicleLand: public VehicleBase
{
public:
    VehicleLand( World &world, const std::string &name = "VehicleLand",
                 const glm::vec3 &pos=glm::vec3(0.0f) );

};


class meat::VehicleAir: public VehicleBase
{
public:
    VehicleAir( World &world,
                  const std::string &name = "VehicleAir",
                  const glm::vec3 &pos=glm::vec3(0.0f) );

};



class meat::VehicleCanoe: public VehicleWater, public iInteractive
{
private:

public:
    VehicleCanoe( World &world, const glm::vec3 &pos=glm::vec3(0.0f) );

};


class meat::VehicleShipPinnacle: public VehicleWater
{
private:

public:
    VehicleShipPinnacle( World &world, const glm::vec3 &pos=glm::vec3(0.0f) );
};


class meat::VehicleTruck: public VehicleLand
{
private:

public:
    VehicleTruck( World &world, const glm::vec3 &pos=glm::vec3(0.0f) );

};




class meat::BehaviourOrientVehicle: public BehaviourBase
{
protected:
    glm::vec3 m_U = glm::vec3(0, 1, 0);
    glm::vec3 m_R = glm::vec3(1, 0, 0);
    glm::vec3 m_F = glm::vec3(0, 0, 1);

    float m_heights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glm::vec3 sample_thing( idk::EngineAPI&, VehicleBase*,
                            std::function<float(float, float)> query,
                            const glm::vec3 &alpha = glm::vec3(1.0f) );

public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) override;
};


class meat::BehaviourOrientVehicleWater: public BehaviourOrientVehicle
{
public:
    using BehaviourOrientVehicle::BehaviourOrientVehicle;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};



class meat::BehaviourDriveVehicle: public BehaviourBase
{
private:

public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};
