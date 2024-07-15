#pragma once

#include <IDKECS/IDKECS.hpp>

namespace meatworld
{
    struct DoorCmp;
    struct WallCmp;

    class  BuildingSys;
}




class meatworld::BuildingSys: public idk::ECS2::System
{
private:
    int m_wall_program = -1;
    int m_wall_model   = -1;

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};



struct meatworld::DoorCmp
{
    int  obj_id  = -1;
    bool is_open = false;
    int  emitter;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};




struct meatworld::WallCmp
{
    int  obj_id = -1;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};

