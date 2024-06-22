#include "meatnet.hpp"


void
meatnet::Base::string_recv( TCPsocket socket, std::string *str )
{
    size_t length;

    if (!generic_recv(socket, &(length)))
    {
        IDK_ASSERT("received == -1", false);
    }

    (*str).resize(length);

    while (running())
    {
        int received = SDLNet_TCP_Recv(socket, str->data(), length*sizeof(char));

        if (received == length*sizeof(char))
        {
            break;
        }

        else if (received == -1)
        {
            IDK_ASSERT("received == -1", false);
        }
    }
}


void
meatnet::Base::string_send( TCPsocket socket, const std::string &str )
{
    if (!generic_send(socket, str.length()))
    {
        IDK_ASSERT("sent == -1", false);
    }

    while (running())
    {
        int sent = SDLNet_TCP_Send(socket, str.data(), str.length()*sizeof(char));

        if (sent == str.length()*sizeof(char))
        {
            break;
        }

        else if (sent == -1)
        {
            IDK_ASSERT("sent == -1", false);
        }
    }
}




void
meatnet::Base::file_recv( TCPsocket socket, meatnet::File &file )
{
    string_recv(socket, &(file.name));

    if (!generic_recv(socket, &(file.nbytes)))
    {
        IDK_ASSERT("received == -1", false);
    }

    void *data = std::malloc(file.nbytes);
    std::cout << "[file_recv] Receiving file \"" << file.name << "\"(" << file.nbytes << " b)\n";

    while (running())
    {
        int received = SDLNet_TCP_Recv(socket, data, file.nbytes);

        if (received == file.nbytes)
        {
            break;
        }

        else if (received == -1)
        {
            IDK_ASSERT("received == -1", false);
        }
    }

    file.data = data;
    std::cout << "[file_recv] Received file\n";
}



void
meatnet::Base::file_send( TCPsocket socket, const std::string &filepath )
{
    size_t nbytes;
    void  *data;

    std::ifstream stream(filepath, std::ios::binary);
    stream.seekg(0, std::ios::end);
    nbytes = stream.tellg();
    stream.seekg(0, std::ios::beg);

    data = std::malloc(nbytes);
    stream.read(reinterpret_cast<char *>(data), nbytes);
    stream.close();


    std::string filename = std::filesystem::path(filepath).filename().string();
    std::cout << "[file_send] Sending file \"" << filename << "\" (" << nbytes << " b)\n";
    string_send(socket, filename);

    if (!generic_send(socket, nbytes))
    {
        IDK_ASSERT("sent == -1", false);
    }

    while (running())
    {
        int sent = SDLNet_TCP_Send(socket, data, nbytes);

        if (sent == nbytes)
        {
            break;
        }

        else if (sent == -1)
        {
            IDK_ASSERT("sent == -1", false);
        }
    }

    std::free(data);
    std::cout << "[file_send] Sent file\n";

}







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


