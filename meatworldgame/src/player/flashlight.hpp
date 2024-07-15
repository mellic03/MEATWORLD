#pragma once


namespace meatworld
{
    class Flashlight;
}


class meatworld::Flashlight
{
public:
    int m_obj_id  = -1;
    int m_emitter = -1;
    bool m_on = false;

    void init( int parent );
    void on();
    void off();
    void toggle();

};
