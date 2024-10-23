#pragma once


namespace meat
{
    class ActorBase;
    class WeaponRanged;
    struct ActorCmp;
    struct WeaponRangedCmp;
}


struct meat::ActorCmp
{
    int obj_id = -1;
    ActorBase *actor;

    size_t  serialize( std::ofstream &stream ) const { return 0; };
    size_t  deserialize( std::ifstream &stream ) { return 0; };
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id ) {  };
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id ) {  };
    static void onObjectCopy( int src_obj, int dst_obj ) {  };
};


struct meat::WeaponRangedCmp
{
    int obj_id = -1;
    WeaponRanged *weapon;

    size_t  serialize( std::ofstream &stream ) const { return 0; };
    size_t  deserialize( std::ifstream &stream ) { return 0; };
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id ) {  };
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id ) {  };
    static void onObjectCopy( int src_obj, int dst_obj ) {  };
};

