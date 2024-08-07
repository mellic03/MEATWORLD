#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>


namespace meatworld
{
    class WeaponBase;

    class MeleeWeapon;
    class RangedWeapon;
    class NPC_RangedWeapon;

    class Hands;
    class Glock;
    class Kar98k;
    class HL2_AR2;

    class NPC_Glock;
    class NPC_HL2_AR2;


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

        float recoil  = 1.0f;
        float aim_fov = 1.0f;
        int   shots   = 1;
        float damage  = 1.0f;

        float inner_prob   = 0.5f;
        float inner_spread = 0.025f;
        float outer_spread = 0.075f;
    };

}



class meatworld::WeaponBase
{
protected:
    int m_obj_id     = -1;
    int m_weapon_obj = -1;
    int m_parent_obj = -1;

    int  m_model     = -1;
    glm::vec3 m_position = glm::vec3(0.0f);

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


class meatworld::NPC_RangedWeapon: public WeaponBase
{
protected:
    WeaponDesc m_desc;
    int m_emitter = -1;
    int m_ignore_obj = -1;

public:

    NPC_RangedWeapon( int parent, const WeaponDesc&, int ignore = -1 );

    virtual WeaponDesc getDesc() = 0;

    virtual void update( idk::EngineAPI&, float dx, float dy );
    virtual void attack( idk::EngineAPI& );

};




class meatworld::RangedWeapon: public WeaponBase
{
protected:
    WeaponDesc m_desc;
    int m_emitter = -1;
    int m_ignore_obj = -1;

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
    Glock( int parent, int ignore = -1 )
    : RangedWeapon(parent, getDesc(), ignore) {  };

    static const std::string name()       { return "Glock"; }
    static const std::string model_path() { return "assets/models/weapons/glock.idkvi"; }

    virtual WeaponDesc getDesc() final;
};



class meatworld::HL2_AR2: public RangedWeapon
{
private:

public:
    HL2_AR2( int parent, int ignore = -1 )
    : RangedWeapon(parent, getDesc(), ignore) {  };

    static const std::string name()       { return "AR2"; }
    static const std::string model_path() { return "assets/models/weapons/hl2-ar2.idkvi"; }

    virtual WeaponDesc getDesc() final;

};




class meatworld::NPC_Glock: public NPC_RangedWeapon
{
private:

public:
    NPC_Glock( int parent, int ignore = -1 )
    : NPC_RangedWeapon(parent, getDesc(), ignore) {  };

    static const std::string name()       { return "Glock"; }
    static const std::string model_path() { return "assets/models/weapons/glock.idkvi"; }

    virtual WeaponDesc getDesc() final;
};



class meatworld::NPC_HL2_AR2: public NPC_RangedWeapon
{
private:

public:
    NPC_HL2_AR2( int parent, int ignore = -1 )
    : NPC_RangedWeapon(parent, getDesc(), ignore) {  };

    static const std::string name()       { return "AR2"; }
    static const std::string model_path() { return "assets/models/weapons/hl2-ar2.idkvi"; }

    virtual WeaponDesc getDesc() final;

};

