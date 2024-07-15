#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>


namespace meatworld
{
    class WeaponBase;

    class MeleeWeapon;
    class RangedWeapon;

    class Hands;
    class Glock;
    class Kar98k;
    class HL2_AR2;

    struct WeaponDesc
    {
        glm::vec3 rest;
        glm::vec3 aim;

        glm::vec3 sway_speed;
        glm::vec3 rest_speed;
        glm::vec3 aim_speed;

        std::string name;
        std::string model_path;

        std::string audio_path = "assets/audio/pistol.wav";

        float recoil = 1.0f;
        float spread = 0.05f;
        int   shots  = 1;
        float damage = 1.0f;
    };

}



class meatworld::WeaponBase
{
protected:
    int m_obj_id     = -1;
    int m_weapon_obj = -1;
    int m_parent_obj = -1;

private:

public:

    WeaponBase( const std::string&, int parent );
    ~WeaponBase();

    virtual void update( idk::EngineAPI&, float dx, float dy ) = 0;
    virtual void attack( idk::EngineAPI& ) = 0;

};



class meatworld::MeleeWeapon: public WeaponBase
{
protected:

public:
    using WeaponBase::WeaponBase;

    virtual void update( idk::EngineAPI&, float dx, float dy ) {  };
    virtual void attack( idk::EngineAPI& ) {  };

};




class meatworld::RangedWeapon: public WeaponBase
{
protected:
    WeaponDesc m_desc;

public:

    RangedWeapon( int parent, const WeaponDesc&, int ignore = -1 );

    virtual WeaponDesc getDesc() = 0;

    virtual void update( idk::EngineAPI&, float dx, float dy );
    virtual void attack( idk::EngineAPI& );

};



class meatworld::Glock: public RangedWeapon
{
private:

public:
    Glock( int parent, int ignore = -1 ): RangedWeapon(parent, getDesc(), ignore) {  };

    static const std::string name()       { return "Glock"; }
    static const std::string model_path() { return "assets/models/weapons/glock.idkvi"; }

    virtual WeaponDesc getDesc() final
    {
        WeaponDesc desc = {
            .rest = glm::vec3(0.1f, -0.05f, 0.0f),
            .aim  = glm::vec3(0.0f, 0.0f, -0.05f),

            .sway_speed = glm::vec3(0.00025f),
            .rest_speed = glm::vec3(0.01f),
            .aim_speed  = glm::vec3(0.1f),

            .name       = "Glock",
            .model_path = "assets/models/weapons/glock.idkvi",
            .audio_path = "assets/audio/pistol.wav",

            .recoil = 0.25f,
            .spread = 0.025f,
            .shots  = 1
        };

        return desc;
    }

};



class meatworld::HL2_AR2: public RangedWeapon
{
private:

public:
    HL2_AR2( int parent, int ignore = -1 ): RangedWeapon(parent, getDesc(), ignore) {  };

    static const std::string name()       { return "AR2"; }
    static const std::string model_path() { return "assets/models/weapons/hl2-ar2.idkvi"; }

    virtual WeaponDesc getDesc() final
    {
        WeaponDesc desc = {
            .rest = glm::vec3(0.1f, -0.05f, -0.1f),
            .aim  = glm::vec3(0.0f,  0.0f,  -0.15f),

            .sway_speed = glm::vec3(0.00025f),
            .rest_speed = glm::vec3(0.01f),
            .aim_speed  = glm::vec3(0.1f),

            .name       = "AR2",
            .model_path = "assets/models/weapons/hl2-ar2.idkvi",
            .audio_path = "assets/audio/pistol.wav",

            .recoil = 0.75f,
            .spread = 0.06f,
            .shots  = 6
        };

        return desc;
    }


};

