#include "sys-building.hpp"

#include <IDKBuiltinCS/sys-audio.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include "sys-weapon.hpp"


idk::EngineAPI *api_ptr;

static int m_RQ = -1;


void
meatworld::BuildingSys::init( idk::EngineAPI &api )
{
    using namespace idk;

    api_ptr = &api;
    auto &ren = api.getRenderer();


    if (m_RQ == -1)
    {
        m_wall_program = ren.createProgram(
            "meatworld-wall", "assets/shaders/", "wall.vs", "wall.fs"
        );

        m_RQ = ren.createRenderQueue("meatworld-wall");
    }

    for (auto &cmp: ECS2::getComponentArray<meatworld::DoorCmp>())
    {
        idk::ModelSys::assignCustomRQ(cmp.obj_id, m_RQ);
    }
}



void
meatworld::BuildingSys::update( idk::EngineAPI &api )
{
    using namespace idk;

    auto &ren = api.getRenderer();

    for (auto &cmp: ECS2::getComponentArray<meatworld::DoorCmp>())
    {
        float  target  = (cmp.is_open) ? glm::radians(90.0f) : 0.0f;
        float &current = TransformSys::getTransformCmp(cmp.obj_id).yaw;
        float  error   = target - current;
    
        current += 0.01f*error;
    }

    for (auto &cmp: ECS2::getComponentArray<meatworld::WallCmp>())
    {
        ModelSys::assignCustomRQ(cmp.obj_id, m_RQ);

        // ren.drawModelRQ(m_RQ, cmp.model, TransformSys::getModelMatrix(cmp.obj_id));
    }
}







size_t
meatworld::DoorCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, is_open);
    return n;
}


size_t
meatworld::DoorCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, is_open);
    return n;
}


void
meatworld::DoorCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::DoorCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::DoorCmp::onObjectCopy( int src_obj, int dst_obj )
{

}






size_t
meatworld::WallCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::WallCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::WallCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    idk::ECS2::setGameObjectName(obj_id, "wall");
    idk::ECS2::giveComponent<idk::StaticRectCmp>(obj_id);
    idk::ECS2::giveComponent<idk::ModelCmp>(obj_id);
    idk::ECS2::giveComponent<HitBoxCmp>(obj_id);

    idk::ModelSys::assignModel(obj_id, "assets/models/proc/tileable-wall.idkvi");
    idk::TransformSys::getXYZScale(obj_id).z = 0.1f;

    idk::ModelSys::assignCustomRQ(obj_id, m_RQ);
}


void
meatworld::WallCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::WallCmp::onObjectCopy( int src_obj, int dst_obj )
{

}
