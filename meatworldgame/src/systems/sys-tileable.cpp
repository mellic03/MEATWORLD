#include "sys-tileable.hpp"



void
meatworld::TileableSys::init( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    ren.createProgram(
        "mw-tileable-floor", "assets/shaders/", "mw-tileable-floor.vs", "mw-tileable-floor.fs"
    );

    m_RQ = ren.createRenderQueue("mw-tileable-floor");

}



void
meatworld::TileableSys::update( idk::EngineAPI &api )
{
    for (auto &cmp: idk::ECS2::getComponentArray<TileableFloorCmp>())
    {
        
    }

}







size_t
meatworld::TileableFloorCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::TileableFloorCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::TileableFloorCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::TileableFloorCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::TileableFloorCmp::onObjectCopy( int src_obj, int dst_obj )
{

}






