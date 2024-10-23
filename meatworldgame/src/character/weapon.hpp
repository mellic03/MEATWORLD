#pragma once

#include "base.hpp"



namespace meat
{
    struct WeaponRangedDesc
    {
        float vel      = 128.0f;
        float mass     = 0.5f;
        float spread   = 0.1f;
        float cooldown = 0.5f;

        float vel_rng      = 32.0f;
        float mass_rng     = 0.0f;
        float spread_rng   = 0.1f;
        float cooldown_rng = 0.25f;
    };

    class WeaponBase;
    class WeaponRanged;
    class WeaponGlock;
    class WeaponAR2;
    class WeaponFlamethrower;

    class ItemWeaponInventory;
}


class meat::WeaponBase: public ItemBase
{
protected:
    int m_emitter;
    int m_obj;
    int m_parent;

private:

public:
    WeaponBase( ActorBase *owner, const std::string &name="WeaponBase" );
    ~WeaponBase();

    int getWeaponID() { return m_obj; }
    virtual void update( idk::EngineAPI&, float dx, float dy ) = 0;
    virtual glm::vec2 attack( idk::EngineAPI&, meat::World& ) = 0;
};


class meat::WeaponRanged: public WeaponBase
{
protected:
    meat::World &worldref;
    float m_timer = 0.0;

private:
    WeaponRangedDesc m_rdesc;

    glm::vec3 m_rest = glm::vec3(0.0f);
    glm::vec3 m_sway = glm::vec3(0.0f);

    static void projectile_callback( idk::EngineAPI&, const glm::vec3&, const glm::vec3& );

public:
    WeaponRanged( meat::World&, ActorBase *owner );

    WeaponRangedDesc &getRangedDesc() { return m_rdesc; }

    virtual void update( idk::EngineAPI&, float dx, float dy );
    virtual glm::vec2 attack( idk::EngineAPI&, meat::World& );
};



class meat::WeaponGlock: public WeaponRanged
{
public:
    WeaponGlock( meat::World&, ActorBase *owner );
};


class meat::WeaponAR2: public WeaponRanged
{
public:
    WeaponAR2( meat::World&, ActorBase *owner );
};


class meat::WeaponFlamethrower: public WeaponAR2
{
private:
    int m_emitter = -1;

public:
    WeaponFlamethrower( meat::World&, ActorBase *owner );
    virtual void update( idk::EngineAPI&, float dx, float dy ) final;
    virtual glm::vec2 attack( idk::EngineAPI&, meat::World& ) final;
};


class meat::ItemWeaponInventory: public ItemBase
{
private:
    bool m_lowered = false;
    std::vector<WeaponBase*> m_weapons;
    WeaponBase *m_weapon;

public:
    ItemWeaponInventory( ActorBase* );
    virtual void update( idk::EngineAPI&, meat::World& ) final;

    void lowerWeapon() { m_lowered = true; }
    void raiseWeapon() { m_lowered = false; }

};
