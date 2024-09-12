#include "animation.hpp"
#include <libidk/idk_math.hpp>



void
meatworld::LegWalkAnimation::advance( float dt )
{
    m_prev_time = m_curr_time;
    m_curr_time += dt;
    m_curr_time = glm::mod(m_curr_time + idk::PI, idk::TAU) - idk::PI;
}


glm::vec3
meatworld::LegWalkAnimation::get( float t, float h, float xs, float zs )
{
    float y = h * ((sinf(t) + 1.0f) / 2.0f);
        //   y = glm::max(y, 0.0f);

    // float xz = sinf(t + idk::PI);
    float xz  = glm::mod(2.0f*(0.5f*t + 0.25f*idk::PI) + idk::PI, idk::TAU) - idk::PI;
          xz  = glm::abs(xz) / (0.5f * idk::PI);
          xz  = 1.0f - xz;

    return glm::vec3(xz, y, xz) * glm::vec3(xs, 1.0f, zs);
}


glm::vec3
meatworld::LegWalkAnimation::rootmotion( float h, float xs, float zs )
{
    return get(m_curr_time, h, xs, zs) - get(m_prev_time, h, xs, zs);
}


