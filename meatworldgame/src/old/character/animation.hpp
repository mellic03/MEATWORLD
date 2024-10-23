#pragma once

#include <libidk/idk_glm.hpp>


namespace meatworld
{
    class ProcAnimation;
    class LegWalkAnimation;
}


class meatworld::ProcAnimation
{
protected:
    float m_prev_time = 0.0f;
    float m_curr_time = 0.0f;

public:

    // virtual glm::vec3 get( float t, float height, float stride ) = 0;
    // virtual glm::vec3 get( float height, float stride ) = 0;
};



class meatworld::LegWalkAnimation: public ProcAnimation
{
public:
    void advance( float dt );

    glm::vec3 get( float t, float height, float xstride, float zstride );

    glm::vec3 get( float h, float xs, float zs )
    {
        return get(m_curr_time, h, xs, zs);
    };

    glm::vec3 rootmotion( float h, float xs, float zs );
};