#pragma once

#include <string>

#include <IDKECS/IDKECS.hpp>
#include <glm/glm.hpp>


namespace meatworld
{
    struct WeaponConfig;

    struct WeaponDesc
    {
        glm::vec4 rest;
        glm::vec4 aim;

        glm::vec3 sway_speed;
        glm::vec3 rest_speed;
        glm::vec3 aim_speed;

        std::string name;
        std::string model_path;

        std::string audio_path = "assets/audio/pistol.wav";

        float recoil  = 1.0f;
        float aim_fov = 1.0f;
        int   shots   = 1;
        float damage  = 1.0f;

        float inner_prob   = 0.5f;
        float inner_spread = 0.025f;
        float outer_spread = 0.075f;
    };

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