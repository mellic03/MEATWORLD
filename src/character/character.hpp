#pragma once

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include <libidk/idk_allocator_id.hpp>
#include <libidk/idk_identifiable.hpp>


namespace idkg
{
    class CharacterFactory;
    class Character;
    class Player;
    class Enemy;
    class Asshole;
}



class idkg::CharacterFactory
{
protected:
    idk::EngineAPI &m_api;
    idk::Allocator_ID<idkg::Character *> m_characters;

public:
    CharacterFactory( idk::EngineAPI& );

    template <typename character_type>
    character_type *createCharacter();


    void update();

};



class idkg::Character: public idk::Identifiable
{
protected:
    friend class idkg::CharacterFactory;

    int m_obj;
    Character( idk::EngineAPI&, int obj_id );

    virtual void update( idk::EngineAPI& ) = 0;
};





class idkg::Player: public idkg::Character
{
protected:
    friend class idkg::CharacterFactory;

    int m_hinge_obj;
    int m_cam_obj;

    Player( idk::EngineAPI&, int obj_id );

    virtual void update( idk::EngineAPI& ) final;

public:

};



class idkg::Enemy: public idkg::Character
{
private:
    friend class idkg::CharacterFactory;

    Enemy( idk::EngineAPI&, int obj_id );
    virtual void update( idk::EngineAPI& ) final;

public:

};



class idkg::Asshole: public idkg::Character
{
private:
    friend class idkg::CharacterFactory;

    using idkg::Character::Character;
    virtual void update( idk::EngineAPI& ) final;

public:

};





#include "./character.inl"

