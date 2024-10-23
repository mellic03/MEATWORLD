#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>


namespace meatworld
{
    class  CharacterBase;
    class  CharacterManager;

    class  CharacterUnFriendly;
    class  CharacterFriendly;
}



void test_behaviour( meatworld::CharacterBase *C )
{
    using namespace meatworld;


}



class meatworld::CharacterManager
{
private:
    std::vector<CharacterBase *> m_characters;

public:

    int createCharacter( CharacterBase *C )
    {
        m_characters.push_back(C);
        return m_characters.size() - 1;
    }

    CharacterBase *getCharacter( int id )
    {
        return m_characters[id];
    }

    void update()
    {
        for (auto *C: m_characters)
        {
            C->update();
        }
    }

};


class meatworld::CharacterBase
{
private:
    friend class meatworld::CharacterManager;

    using callback_type = std::function<void(CharacterBase*)>;
    std::vector<callback_type> m_behaviours;

    void update()
    {
        for (auto &fn: m_behaviours)
        {
            fn();
        }
    }


public:
    glm::vec3 m_position;

    CharacterBase( const glm::vec3 &position, float yaw )
    :   m_position(position)
    {

    }

    void giveBehaviour( const callback_type &fn )
    {
        m_behaviours.push_back(fn);
    }

};



class meatworld::CharacterUnFriendly: public CharacterBase
{
public:
    using CharacterBase::CharacterBase;

};



class meatworld::CharacterFriendly: public CharacterBase
{
public:
    using CharacterBase::CharacterBase;

};

