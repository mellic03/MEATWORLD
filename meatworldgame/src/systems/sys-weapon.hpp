#pragma once

#include <IDKECS/IDKECS.hpp>
#include "../player/weapon.hpp"


namespace meatworld
{
    struct WeaponConfig;

    struct Projectile
    {
        bool enabled;
        bool aim;
        glm::vec3 pos;
        glm::vec3 dir;
        meatworld::WeaponDesc desc;
        std::function<void(glm::vec3, glm::vec3)> callback;
        int ignore = -1;
    };

    struct WeaponCmp;
    struct HitSphereCmp;
    struct HitBoxCmp;
    struct CharacterHitBoxCmp;

    class WeaponSys;
}

class meatworld::WeaponSys: public idk::ECS2::System
{
private:
    static bool _raycast( idk::EngineAPI&, const Projectile&, const glm::vec3&, const glm::vec3&,
                          glm::vec3 *hit, glm::vec3 *N, int *obj_id );

public:
    virtual void init   ( idk::EngineAPI & ) final;
    virtual void update ( idk::EngineAPI & ) final;

    static void config( int obj_id, const WeaponDesc& );
    static void fire( int obj_id);

    static void createProjectile( const glm::vec3&, const glm::vec3&, const WeaponDesc&, bool aim,
                                  std::function<void(glm::vec3, glm::vec3)> callback, int ignore = -1 );
};




// struct meatworld::WeaponConfig
// {
//     float spread = 0.05f;
//     int   shots  = 6;
// };



struct meatworld::WeaponCmp
{
    int obj_id = -1;
    int ignore_obj = -1;
    bool fire = false;
    WeaponDesc desc;

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