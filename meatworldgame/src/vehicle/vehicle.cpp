#include "vehicle.hpp"
#include "../world/world.hpp"
#include "../character/behaviour.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKIO/IDKIO.hpp>

#include "../systems/sys-vehicle.hpp"

using namespace meat;



VehicleShipPinnacle::VehicleShipPinnacle( World &world, const glm::vec3 &pos )
:   VehicleWater(world, "Pinnacle", pos)
{
    VehicleDesc desc = {
        .seat_pos    = glm::vec3(0.0f, 1.5f, -1.5f),
        .model_pos   = glm::vec3(0.0f, -0.85f, 0.0f),
        .model_scale = glm::vec3(1.0f),
        .model_path  = "assets/models/vehicles/ship-pinnacle.idkvi",
        .track       = 5.0f,
        .wheelbase   = 15.0f,
        .suspension  = 4.0f
    };
    setVehicleDesc(desc);
}

VehicleCanoe::VehicleCanoe( World &world, const glm::vec3 &pos )
:   VehicleWater(world, "Canoe", pos)
{
    VehicleDesc desc = {
        .seat_pos    = glm::vec3(0.0f, 1.0f, 1.0f),
        .model_pos   = glm::vec3(0.0f, 0.5f, 0.0f),
        .model_scale = glm::vec3(1.0f),
        .model_path  = "assets/models/vehicles/canoe.idkvi",
        .track       = 1.0f,
        .wheelbase   = 3.0f,
        .suspension  = 1.0f
    };
    setVehicleDesc(desc);

}


VehicleTruck::VehicleTruck( World &world, const glm::vec3 &pos )
:   VehicleLand(world, "Truck", pos)
{
    VehicleDesc desc = {
        .seat_pos    = glm::vec3(0.8f, 2.75f, -3.5f),
        .model_pos   = glm::vec3(0.0f, -0.25f, 0.0f),
        .model_scale = glm::vec3(2, 2, 2),
        .model_path  = "assets/models/vehicles/fighter.idkvi",
        .track       = 2.9f,
        .wheelbase   = 8.0f,
        .suspension  = 1.65f
    };
    setVehicleDesc(desc);
}



VehicleWater::VehicleWater( World &world, const std::string &name, const glm::vec3 &pos )
:   VehicleBase(world, name, pos)
{
    removeBehaviour<BehaviourOrientVehicle>();
    giveBehaviour<BehaviourOrientVehicleWater>();
}


VehicleLand::VehicleLand( World &world, const std::string &name, const glm::vec3 &pos )
:   VehicleBase(world, name, pos)
{
    giveBehaviour<BehaviourOrientVehicle>();

}


VehicleBase::VehicleBase( World &world, const std::string &name, const glm::vec3 &pos )
:   ActorBase(world, name, pos, 0.0f),
    m_driver(nullptr)
{
    auto adesc = getActorDesc();
         adesc.height = 0.0f;
    setActorDesc(adesc);
    // setActorDesc({0.0f, 1.0f, 1.0f});

    // idk::ECS2::giveComponent<idk::AudioEmitterCmp>(m_attach_obj);
    idk::ECS2::giveComponent<meat::VehicleCmp>(m_root_obj);
    idk::ECS2::getComponent<meat::VehicleCmp>(m_root_obj).actor = this;
    idk::ECS2::giveComponent<idk::ModelCmp>(m_attach_obj);

    idk::TransformSys::getLocalPosition(m_attach_obj) = glm::vec3(0.0f, 1.0f, 0.0f);
    idk::ModelSys::assignModel(m_attach_obj, "assets/models/ship2.idkvi");
    // idk::AudioSys::assignSound(m_attach_obj, "assets/audio/vehicle/engine-loop-1-normalized.wav");

    giveBehaviour<BehaviourPlayerKeyInput>();
    giveBehaviour<BehaviourPlayerMouseInput>();
    disableBehaviour<BehaviourPlayerKeyInput>();
    disableBehaviour<BehaviourPlayerMouseInput>();

    giveBehaviour<BehaviourGravity>();
    giveBehaviour<BehaviourCollideWithTerrain>();
}


void
VehicleBase::setVehicleDesc( const VehicleDesc &desc )
{
    m_desc = desc;

    idk::ModelSys::assignModel(m_attach_obj, desc.model_path);
    idk::TransformSys::getLocalPosition(m_attach_obj) = desc.model_pos;
    idk::TransformSys::getXYZScale(m_attach_obj)      = desc.model_scale;
}



void
VehicleBase::update( idk::EngineAPI &api, meat::World &world )
{
    float dt   = api.dtime();
    auto &vdesc = getVehicleDesc();

    m_steering += dt * vdesc.steer_damp * (0.0f - m_steering);
    // m_speed    += dt * cmp.decceleration * (0.0f - m_speed);
    // m_speed     = glm::clamp(m_speed, -cmp.max_speed, +cmp.max_speed);

    float &yaw0 = idk::TransformSys::getTransformCmp(m_root_obj).yaw = m_steering;
    float &yaw1 = idk::TransformSys::getTransformCmp(m_attach_obj).yaw;

    ActorBase::update(api, world);
}



void
VehicleBase::drive( ActorBase *C )
{
    m_driver = C;

    // idk::ECS2::giveChild(this->getID(), C->getID());
    // C->setLocalPosition(m_desc.seat_pos);

    C->multiplyVel(glm::vec3(0.0f));
    C->multiplyForce(glm::vec3(0.0f));


    C->disableBehaviour<BehaviourGravity>();
    C->disableBehaviour<BehaviourCollideWithTerrain>();
    C->disableBehaviour<BehaviourPlayerKeyInput>();
    this->enableBehaviour<BehaviourPlayerKeyInput>();

    world.EM.emit("VehicleBase::drive", C);
    world.EM.emit("Vehicle" + getName() + "::drive", C);
}


void
VehicleBase::undrive()
{
    auto *C = m_driver;

    world.EM.emit("VehicleBase::undrive", C);
    world.EM.emit("Vehicle" + getName() + "::undrive", C);

    // idk::ECS2::removeChild(this->getAttachID(), C->getID());
    // idk::TransformSys::setWorldPosition(C->getID(), this->getPosition());

    C->enableBehaviour<BehaviourGravity>();
    C->enableBehaviour<BehaviourCollideWithTerrain>();
    C->enableBehaviour<BehaviourPlayerKeyInput>();
    this->disableBehaviour<BehaviourPlayerKeyInput>();

    m_driver = nullptr;

}



void
BehaviourDriveVehicle::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    glm::vec3 cpos = C->getPosition();
    auto *V = world.getNearest<VehicleBase>(cpos, 5.0f);

    if (V == nullptr)
    {
        return;
    }

    auto *driver = V->getDriver();

    if (driver == nullptr)
    {
        world.textPrompt("[E] Enter " + V->getName());
        // world.renderText(2, 2, "[E] Enter " + V->getName());

        if (idkio::keyTapped(idk::Keycode::E))
        {
            V->drive(C);
        }
    }

    else if (driver == C)
    {
        idk::TransformSys::moveTowards(driver->getID(), V->getAttachID(), 0.1f);

        // world.renderText(3, 2, "[E] Exit " + V->getName());

        if (idkio::keyTapped(idk::Keycode::E))
        {
            V->undrive();
        }
        return;
    }

}



glm::vec3
BehaviourOrientVehicle::sample_thing( idk::EngineAPI &api, VehicleBase *V,
                                      std::function<float(float, float)> query,
                                      const glm::vec3 &alpha )
{
    auto &ren  = api.getRenderer();
    auto &desc = V->getVehicleDesc();

    glm::mat4 M   = idk::TransformSys::getModelMatrix(V->getAttachID()); // V->getTransform();
    glm::vec3 avg = glm::vec3(0.0f);

    glm::vec3 wheels[4] = {
        M * glm::vec4(-0.5*desc.track, -desc.suspension, -0.5*desc.wheelbase, 1.0f),
        M * glm::vec4(+0.5*desc.track, -desc.suspension, -0.5*desc.wheelbase, 1.0f),
        M * glm::vec4(-0.5*desc.track, -desc.suspension, +0.5*desc.wheelbase, 1.0f),
        M * glm::vec4(+0.5*desc.track, -desc.suspension, +0.5*desc.wheelbase, 1.0f)
    };

    for (int i=0; i<4; i++)
    {
        float height = query(wheels[i].x, wheels[i].z);

        m_heights[i] = glm::mix(m_heights[i], height, alpha.y);
        wheels[i].y  = m_heights[i];

        avg += wheels[i] / 4.0f;
    }

    glm::vec3 l_avg = glm::mix(wheels[0], wheels[2], 0.5f);
    glm::vec3 r_avg = glm::mix(wheels[1], wheels[3], 0.5f);
    glm::vec3 f_avg = glm::mix(wheels[0], wheels[1], 0.5f);
    glm::vec3 b_avg = glm::mix(wheels[2], wheels[3], 0.5f);

    glm::vec3 F = glm::normalize(f_avg - b_avg);
    glm::vec3 R = glm::normalize(r_avg - l_avg);
    glm::vec3 U = glm::normalize(glm::cross(R, F));
              F = glm::cross(U, R);

    m_R = glm::normalize(glm::mix(m_R, R, alpha.x));
    m_F = glm::normalize(glm::mix(m_F, F, alpha.z));
    m_U = glm::normalize(glm::mix(m_U, U, 1.0f));
    m_F = -glm::cross(m_U, m_R);

    return avg;
}



void
BehaviourOrientVehicle::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    auto *V  = dynamic_cast<VehicleBase*>(C);

    glm::vec3 avg = sample_thing(api, V, idk::TerrainRenderer::heightQuery);

    C->setRotation(glm::mat3(m_R, m_U, m_F));

    glm::vec3 &pos = V->getLocalPosition();
               pos.y += glm::max(avg.y - pos.y, 0.0f);
}


void
BehaviourOrientVehicleWater::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
{
    float dt = api.dtime();
    auto *V  = dynamic_cast<VehicleBase*>(C);
    auto &adesc = V->getActorDesc();
    auto &vdesc = V->getVehicleDesc();


    static auto height_fn = []( float x, float z )
    {
        float h0 = idk::TerrainRenderer::heightQuery(x, z);
        float h1 = idk::TerrainRenderer::waterHeightQuery(x, z);
        return std::max(h0, h1);
    };

    glm::vec3 avg = sample_thing(api, V, height_fn, vdesc.spring);


    glm::vec3 pos0 = idk::TransformSys::getLocalPosition(V->getAttachID());

    glm::mat4 T = glm::mat4(glm::mat3(m_R, m_U, m_F));
              T[3] = glm::vec4(pos0, 1.0f);

    idk::TransformSys::getTransform(C->getAttachID()) = idk::Transform::fromGLM(T);

    // C->setRotation();

    glm::vec3 &pos = idk::TransformSys::getLocalPosition(V->getID());
               pos.y += glm::max(avg.y - pos.y, 0.0f);

}


void
VehicleBase::move( const glm::vec3 &delta )
{
    if (m_driver == nullptr)
    {
        return;
    }

    // float dt  = api.dtime();
    auto &vdesc = getVehicleDesc();

    glm::vec3 front = idk::TransformSys::getFront(m_attach_obj);
    glm::vec3 &pos  = idk::TransformSys::getLocalPosition(m_root_obj);

    // m_steering -= dt * vdesc.steer_spd * delta.x * glm::sign(fabs(delta.z));

    m_steering -= vdesc.steer_spd * glm::sign(delta.x);
    this->addForce(-vdesc.acc * delta.z * front);

}
