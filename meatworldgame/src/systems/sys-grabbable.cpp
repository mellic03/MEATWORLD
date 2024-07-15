#include "sys-grabbable.hpp"

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <libidk/idk_log.hpp>

#include "../game.hpp"



namespace
{
    std::map<std::string, std::function<void(idk::EngineAPI&, int)>> m_callbacks;
}


void
idk::GrabbableSys::init( idk::EngineAPI &api )
{
    m_callbacks[""] = [](idk::EngineAPI&, int){};
}


void
idk::GrabbableSys::update( idk::EngineAPI &api )
{
    for (auto &cmp: idk::ECS2::getComponentArray<idk::PickupCmp>())
    {
        _update_pickup(api, cmp);
    }
}




void
idk::GrabbableSys::_update_pickup( idk::EngineAPI &api, PickupCmp &cmp )
{
    using namespace idk;

    if (cmp.curr_quantity <= 0)
    {
        return;
    }


    auto *player = MeatWorldGame::player;

    if (player == nullptr)
    {
        return;
    }

    glm::vec3 cam_pos    = TransformSys::getPositionWorldspace(player->m_cam_obj);
    glm::vec3 cam_dir    = TransformSys::getFront(player->m_cam_obj);
    glm::vec3 sphere_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
    float     sphere_rad = TransformSys::getUniformScale(cmp.obj_id);


    if (glm::distance2(cam_pos, sphere_pos) > 1.0f)
    {
        return;
    }

    if (idk::geometry::raySphereIntersects(cam_pos, cam_dir, sphere_pos, sphere_rad) == false)
    {
        return;
    }

    auto &ren = api.getRenderer();
    int w = ren.width();
    int h = ren.height();

    if (cmp.curr_quantity > 1)
    {
        idkui::TextManager::text(w/2, h/2) << "[E] pick up " << cmp.name
                                           << " (" << cmp.curr_quantity  << ")";
    }

    else
    {
        idkui::TextManager::text(w/2, h/2) << "[E] pick up " << cmp.name;
    }


    if (api.getEventSys().keylog().keyTapped(idk::Keycode::E) == false)
    {
        return;
    }

    m_callbacks[cmp.callback](api, cmp.obj_id);
    cmp.curr_quantity -= 1;

}



void
idk::GrabbableSys::registerCallback( const std::string &name,
                                     std::function<void(idk::EngineAPI&, int)> callback )
{
    m_callbacks[name] = callback;
    LOG_INFO() << "[meatworld::GrabbableSys] Registered callback \"" << name << "\".";
}





size_t
idk::PickupCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, max_quantity);
    n += idk::streamwrite(stream, name);
    n += idk::streamwrite(stream, callback);
    return n;
}


size_t
idk::PickupCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, max_quantity);
    n += idk::streamread(stream, name);
    n += idk::streamread(stream, callback);

    curr_quantity = max_quantity;

    if (m_callbacks.contains(callback) == false)
    {
        LOG_WARN() << "[meatworld::PickupCmp] Callback \"" << callback << "\" does not exist.";
        // callback = "";
    }

    return n;
}


void
idk::PickupCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PickupCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PickupCmp::onObjectCopy( int src_obj, int dst_obj )
{

}







size_t
idk::GrabbableCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
idk::GrabbableCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
idk::GrabbableCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::GrabbableCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::GrabbableCmp::onObjectCopy( int src_obj, int dst_obj )
{

}



