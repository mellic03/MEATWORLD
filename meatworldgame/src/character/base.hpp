#pragma once

#include <string>
#include <vector>
#include <array>
#include <map>
#include <functional>
#include <glm/glm.hpp>

#include "manager.hpp"
#include "../behaviour/behaviour.hpp"


namespace idk
{
    class EngineAPI;
}


namespace meat
{
    class  iUpdateable;
    class  iInteractive;

    class  BehaviourBase;
    class  ItemBase;
    class  ItemWrapper;

    struct DurationForce
    {
        glm::vec3 v = glm::vec3(0.0f);
        float     t = 0.0f;
    };

    struct ActorDesc;
    class  ActorBase;
    class  EventManager;
    class  World;
}


class meat::iUpdateable
{
public:
    virtual void update( idk::EngineAPI&, meat::World& ) {  };

};

class meat::iInteractive
{
private:
    using callback_type = std::function<void(idk::EngineAPI&, meat::World&)>;

    callback_type m_callback = [](idk::EngineAPI&, meat::World&)
    {
        std::cout << "iInteractive::callback\n";
    };

public:
    // iInteractive( const callback_type &callback )
    // :   m_callback(callback)
    // {

    // }

    virtual void onInteract( const callback_type &fn ) { m_callback = fn; };
};




class meat::BehaviourBase
{
private:
    bool m_enabled = true;

protected:
    ActorBase *m_actor;

public:
    BehaviourBase( ActorBase *A ): m_actor(A) {  };

    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) = 0;

    bool enabled() const  { return m_enabled; }
    void enable()  { m_enabled = true; }
    void disable() { m_enabled = false; }

};




struct meat::ActorDesc
{
    float height   = 1.75f;
    float walk_spd = 1.0f;
    float run_spd  = 2.0f;

    float mass     = 1.0f;
    float buoyancy = 0.85f;

    float jmp_imp  = 2.0f;
    float jmp_acc  = 4.0f;
    float jmp_dur  = 1.0f / 4.0f;

    float imp_damp = 0.0f;
    float acc_damp = 0.99f;
    float vel_damp = 0.99f;

    float acc_max  = 32.0f;
    float vel_max  = 32.0f;
};



class meat::ActorBase: public iUpdateable
{
private:
    friend class World;

    ActorDesc                        m_desc;
    std::map<size_t, BehaviourBase*> m_behaviours;
    std::map<size_t, ItemBase *>     m_items;
    std::string m_name = "";

    glm::vec3 m_prev_world[2] = {glm::vec3(0.0f), glm::vec3(0.0f)};
    glm::vec3 m_prev_local[2] = {glm::vec3(0.0f), glm::vec3(0.0f)};
    glm::vec3 m_curr_world    = glm::vec3(0.0f);
    glm::vec3 m_curr_local    = glm::vec3(0.0f);

    float     m_accum         = 0.0f;
    float     m_alpha         = 0.0f;

    glm::vec4 m_ang = glm::vec4(0.0f);
    glm::vec3 m_imp = glm::vec3(0.0f);
    glm::vec3 m_acc = glm::vec3(0.0f);
    glm::vec3 m_vel = glm::vec3(0.0f);

    // int m_acc_idx = 0;
    // std::array<DurationForce, 16> m_dur_acc;
    bool m_grounded = false;


protected:
    int m_root_obj;
    int m_attach_obj;
    BNode *m_btree;


public:
    World &world;

         ActorBase( World &w,
                    const std::string &name = "ActorBase",
                    const glm::vec3   &pos  = glm::vec3(0.0f),
                    float yaw = 0.0f );

    virtual ~ActorBase();

    virtual void update( idk::EngineAPI&, meat::World& ) override;

    const std::string &getName() const { return m_name; }
    const int getID() const { return m_root_obj; }
    const int getAttachID() const { return m_attach_obj; }

    virtual bool canInteract ( idk::EngineAPI&, meat::World&, uint32_t ) { return false; };
    virtual void onInteract  ( idk::EngineAPI&, meat::World&, uint32_t ) {  };

    void setBehaviour( BNode *btree )
    {
        m_btree = btree;
    }

    BNode *getBehaviour()
    {
        return m_btree;
    }

    template <typename behaviour_type>
    void giveBehaviour()
    {
        size_t key = typeid(behaviour_type).hash_code();
        m_behaviours[key] = dynamic_cast<BehaviourBase*>(new behaviour_type(this));
    }

    template <typename behaviour_type>
    void removeBehaviour()
    {
        if (getBehaviour<behaviour_type>())
        {
            size_t key = typeid(behaviour_type).hash_code();
            m_behaviours.erase(key);
        }
    }

    template <typename behaviour_type>
    BehaviourBase *getBehaviour()
    {
        size_t key = typeid(behaviour_type).hash_code();
    
        if (m_behaviours.contains(key) == false)
        {
            LOG_WARN() << "Character does not have behaviour: " << typeid(behaviour_type).name();
            return nullptr;
        }

        return m_behaviours[key];
    }

    template <typename behaviour_type>
    void disableBehaviour()
    {
        auto *B = getBehaviour<behaviour_type>();
        if (B) { B->disable(); }
    }

    template <typename behaviour_type>
    void enableBehaviour()
    {
        auto *B = getBehaviour<behaviour_type>();
        if (B) { B->enable(); }
    }



    template <typename item_type>
    void giveItem()
    {
        size_t key = typeid(item_type).hash_code();
        m_items[key] = new item_type(this);
    }


    template <typename item_type, typename... Args>
    void giveItem( Args&&... args )
    {
        size_t key = typeid(item_type).hash_code();
        m_items[key] = new item_type(this, std::forward<Args>(args)...);
    }


    template <typename item_type>
    item_type *getItem()
    {
        size_t key = typeid(item_type).hash_code();
    
        if (m_items.contains(key) == false)
        {
            LOG_WARN() << "Character does not have item: " << typeid(item_type).name();
            return nullptr;
        }

        return dynamic_cast<item_type*>(m_items[key]);
    }


    ActorDesc &getActorDesc() { return m_desc; }
    void setActorDesc( const ActorDesc &desc ) { m_desc = desc; };

    float getHeight()    const { return m_desc.height;   }
    float getWalkSpeed() const { return m_desc.walk_spd; }
    float getRunSpeed()  const { return m_desc.run_spd;  }

    glm::mat4 getTransform();
    void      setTransform( const glm::mat4& );
    void      setRotation( const glm::mat3& );

    glm::vec3  getFront();

    glm::vec3  getPosition();
    glm::vec3 &getVelocity() { return m_vel; };
    glm::vec3 &getAcc()      { return m_acc; };

    glm::vec3 &getLocalPosition();
    void       setLocalPosition( const glm::vec3& );

    void translateWorld( const glm::vec3& );
    void translateLocal( const glm::vec3& );

    void addVel          ( const glm::vec3 &v )  { m_vel += v; };
    void multiplyVel     ( const glm::vec3 &v )  { m_vel *= v; };

    void addForce        ( const glm::vec3 &v )  { m_acc += v; };
    void multiplyForce   ( const glm::vec3 &v )  { m_acc *= v; };

    void setAngular      ( const glm::vec4 &v )  { m_ang  = v; };
    void addAngular      ( const glm::vec4 &v )  { m_ang += v; };
    void multiplyAngular ( const glm::vec4 &v )  { m_ang *= v; };

    // void addForceFor  ( const glm::vec3 &v, float t )
    // {
    //     m_acc_idx = (m_acc_idx + 1) % m_dur_acc.size();
    //     m_dur_acc[m_acc_idx] = {v, t};
    // }


    void addImpulse     ( const glm::vec3 &v )  { m_imp += v; };
    // void addImpulseLocal( const glm::vec3 &v )  { m_imp += v; };
    void multiplyImpulse( const glm::vec3 &v )  { m_imp *= v; };

    void setGrounded ( bool b ) { m_grounded = b;    }
    bool isGrounded  (        ) { return m_grounded; }

    virtual void look( const glm::vec2& ) {  };

    virtual void move( const glm::vec3& );
    virtual void walk( const glm::vec3& );
    virtual void run ( const glm::vec3& );

    virtual void moveTo( const glm::vec3& ) {  };
    virtual void jump() {  };
};






class meat::ItemBase: public iUpdateable
{
private:
    std::string m_model_path = "";

protected:
    ActorBase *m_owner;
    void setModelPath( const std::string &str ) { m_model_path = str; } 

public:
    ItemBase( ActorBase *owner=nullptr );
    virtual ~ItemBase();

    ActorBase *getOwner( ) const { return m_owner; }
    void       setOwner( ActorBase *A ) { m_owner = A;    }

    const std::string &getModelPath() const { return m_model_path; }

};



class meat::ItemWrapper: public iInteractive, public ActorBase
{
private:
    ItemBase *m_item;
    int       m_model = -1;

public:
    ItemWrapper( World &world, const glm::vec3&, ItemBase* );
    virtual void update( idk::EngineAPI&, meat::World& ) override;

};


