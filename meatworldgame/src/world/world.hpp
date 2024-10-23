#pragma once

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <functional>
#include <glm/glm.hpp>

#include <libidk/idk_eventmanager.hpp>

#include <IDKGraphics/UI/idk_ui.hpp>
#include "../physics/rigidbody.hpp"
#include "../character/base.hpp"
#include "../vehicle/vehicle.hpp"
#include "../npc/drone.hpp"


#include "ui/ingame-ui.hpp"
#include "ui/menu-ui.hpp"


namespace idk
{
    class EngineAPI;
}


namespace meat
{
    class GameUI;
    class World;

    using EventManagerUIType = idk::EventManager<std::string, idkui::Element*>;
}




class meat::GameUI
{
private:
    std::stack<idkui::Element*> m_stack;

public:
    std::vector<uint32_t> textures;


    GameUI();

    std::string text = "default text";
    EventManagerUIType EM;

    idkui::Stack         *menu_ui       = nullptr;
    idkui::Element       *ingame_ui     = nullptr;
    idkui::Element       *inventory_ui  = nullptr;

    idkui::TextAbsolute  *onscreen_text = nullptr;
    idkui::ImageAbsolute *crosshair     = nullptr;

    idkui::ImageAbsolute *hitmarker_img = nullptr;
    std::vector<glm::vec3> hitmarker_pos;


    idkui::Element *ingame_menu = nullptr;
    idkui::Element *multiplayer = nullptr;
    idkui::Element *settings    = nullptr;


    void clear()
    {
        while (m_stack.empty() == false)
        {
            m_stack.pop();
        }
    }

    void pushMenu( idkui::Element *node )
    {
        m_stack.push(node);
    }

    idkui::Element *popMenu()
    {
        if (m_stack.empty())
        {
            return nullptr;
        }

        auto *ptr = m_stack.top();
                    m_stack.pop();
        return ptr;
    }

    idkui::Element *topMenu()
    {
        if (m_stack.empty())
        {
            return nullptr;
        }

        return m_stack.top();
    }

};



class meat::World
{
public:
    GameUI ui;

private:
    idk::EngineAPI &apiref;


public:
    idk::EventManager<std::string, ActorBase*> EM;

         World( idk::EngineAPI& );
        ~World();

    void update( idk::EngineAPI& );
    void textPrompt( const std::string& );
    void hitMarker( const glm::vec3& );


private:
    std::vector<PhysicsBody*> m_physicsbodies;
    std::vector<ActorBase*> m_objects;
    std::map<size_t, std::vector<ActorBase*>> m_objects_sorted;

public:

    template <typename T, typename... Args>
    void addPhysicsBody( Args&&... args )
    {
        PhysicsBody *ptr = dynamic_cast<PhysicsBody*>(new T(std::forward<Args>(args)...));
        size_t key = typeid(T).hash_code();
        m_physicsbodies.push_back(ptr);
    }


    template <typename U, typename... Args>
    U *addObject( Args&&... args )
    {
        U *obj = new U(*this, std::forward<Args>(args)...);
        ActorBase *ptr = dynamic_cast<ActorBase*>(obj);
        size_t key = typeid(U).hash_code();

        m_objects.push_back(ptr);

        if (dynamic_cast<ItemWrapper*>(ptr))
        {
            key = typeid(ItemWrapper).hash_code();
            m_objects_sorted[key].push_back(ptr);
        }

        else if (dynamic_cast<VehicleBase*>(ptr))
        {
            key = typeid(VehicleBase).hash_code();
            m_objects_sorted[key].push_back(ptr);
        }

        else
        {
            m_objects_sorted[key].push_back(ptr);
        }

        return obj;
    }

    void clear();


    template <typename U>
    U *getNearest( const glm::vec3 &pos, float range )
    {
        ActorBase *nearest = nullptr;
        float dist = range;

        size_t key = typeid(U).hash_code();
        for (ActorBase *C: m_objects_sorted[key])
        {
            glm::vec3 cpos = C->getPosition();
        
            float d = glm::distance(pos, cpos);

            if (d < dist)
            {
                dist = d;
                nearest = C;
            }
        }

        return dynamic_cast<U*>(nearest);
    }


    PhysicsBody *nearestPhysicsBody( const glm::vec3 &pos )
    {
        PhysicsBody *nearest = nullptr;
        float dist = 1000.0f;

        for (auto *body: m_physicsbodies)
        {
            float d = glm::distance(pos, body->getPosition());

            if (d < dist)
            {
                dist = d;
                nearest = body;
            }
        }

        return nearest;
    }


};
