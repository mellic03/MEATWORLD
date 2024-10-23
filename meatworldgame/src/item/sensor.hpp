#pragma once

#include <stack>

namespace meat
{
    template <typename T>
    class Sensor;
}


template <typename T>
class meat::Sensor
{
private:
    float         m_tickrate;
    std::stack<T> m_readings;


public:
    Sensor( float tickrate = 1.0f/4.0f )
    :   m_tickrate(tickrate),
        m_ready(false)
    {

    }

    virtual bool read( T *data )
    {
        if (m_readings.empty() == false)
        {
            *data = m_readings.top();
                    m_readings.pop();
            return true;
        }
    
        return false;
    }

};

