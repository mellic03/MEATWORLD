#pragma once

#include <vector>
#include <map>
#include <functional>


namespace meat
{
    class FState;
    class FStateExample1;
    class FStateExample2;

    class FTransition;
    class FSA;
}

class meat::FState
{
public:
    virtual meat::FState *update() { return nullptr };
};



class meat::FStateExample1: public meat::FState
{
public:

    virtual meat::FState *update()
    {
        if (0)
        {
            return new meat::FStateExample2;
        }

        return nullptr;
    }

};


class meat::FStateExample2: public meat::FState
{
public:

    virtual meat::FState *update()
    {
        if (0)
        {
            return new meat::FStateExample1;
        }

        return nullptr;
    }

};



class meat::FSA
{
private:
    int m_current_state;

    std::vector<std::function<void()>> m_states;
    std::map<int, std::map<int, std::function<bool()>>> m_transitions;

public:

    int addState( int state, const std::function<void()> &fn )
    {
        m_states.push_back(fn);
        return m_states.size() - 1;
    }

    void addTransition( int fromstate, int tostate, const std::function<bool()> &fn )
    {
        m_transitions[fromstate][tostate] = fn;
    }

    void changeState( int tostate )
    {
        m_current_state = tostate;
    }

    void update()
    {
        m_states[m_current_state]();

        for (auto &[tostate, fn]: m_transitions[m_current_state])
        {
            if (fn())
            {
                changeState(tostate);
                break;
            }
        }
    }

};


