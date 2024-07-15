#pragma once

#include <IDKECS/IDKECS.hpp>


namespace meatworld
{
    struct WeaponConfig;

    struct WeaponCmp;
    struct HitSphereCmp;
    struct HitBoxCmp;
    struct CharacterHitBoxCmp;

    class WeaponSys;
}

class meatworld::WeaponSys: public idk::ECS2::System
{
private:
    static void _raycast( idk::EngineAPI&, meatworld::WeaponCmp&, const glm::vec3&, const glm::vec3& );

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void config( int obj_id, const WeaponConfig& );
    static void fire( int obj_id );
};




struct meatworld::WeaponConfig
{
    float spread = 0.05f;
    int   shots  = 6;
};



struct meatworld::WeaponCmp
{
    int obj_id = -1;
    int ignore_obj = -1;
    bool fire = false;

    WeaponConfig config;

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};



struct meatworld::HitSphereCmp
{
    int obj_id = -1;
    bool is_hit = false;
    glm::vec3 hit = glm::vec3(0.0f);

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};



struct meatworld::HitBoxCmp
{
    int obj_id = -1;
    bool is_hit = false;
    glm::vec3 hit = glm::vec3(0.0f);

    std::function<void()> callback = [](){};

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};


struct meatworld::CharacterHitBoxCmp
{
    int obj_id = -1;
    bool is_hit = false;
    glm::vec3 hit = glm::vec3(0.0f);

    std::function<void()> callback = [](){};

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};