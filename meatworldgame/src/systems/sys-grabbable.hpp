#pragma once

#include <IDKECS/IDKECS.hpp>

namespace idk
{
    struct GrabbableCmp;
    struct PickupCmp;
    class  GrabbableSys;
}




class idk::GrabbableSys: public idk::ECS2::System
{
private:
    static void _update_pickup( idk::EngineAPI&, PickupCmp& );

public:
    virtual void init   ( idk::EngineAPI& ) final;
    virtual void update ( idk::EngineAPI& ) final;

    static void registerCallback( const std::string&, std::function<void(idk::EngineAPI&, int)> );

};



struct idk::GrabbableCmp
{
    int obj_id = -1;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};




struct idk::PickupCmp
{
    int obj_id        = -1;
    int max_quantity  = 1;
    int curr_quantity = 1;

    std::string name     = "";
    std::string callback = "";

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};

