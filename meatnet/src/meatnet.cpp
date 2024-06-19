#include "meatnet.hpp"




meatnet::HandshakeBuffer
meatnet::HandshakeBuffer_new()
{
    HandshakeBuffer buffer;

    buffer.response = -1;
    std::memset(buffer.header, '\0', sizeof(buffer.header));
    std::memset(buffer.body,   '\0', sizeof(buffer.body));

    return buffer;
}



meatnet::ClientBuffer
meatnet::ClientBuffer_new( int player_id )
{
    ClientBuffer buffer;

    buffer.response = -1;
    std::memset(buffer.header, '\0', sizeof(buffer.header));
    std::memset(buffer.body,   '\0', sizeof(buffer.body));

    buffer.player_id = player_id;
    buffer.position  = glm::vec3(0.0f);
    buffer.yaw       = 0.0f;
    buffer.action    = ACTION_NONE;

    return buffer;
}


