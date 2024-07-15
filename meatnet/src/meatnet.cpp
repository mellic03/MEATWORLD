#include "meatnet.hpp"


// void
// meatnet::Base::string_recv( TCPsocket socket, std::string *str )
// {
//     size_t length;

//     if (!generic_recv(socket, &(length)))
//     {
//         IDK_ASSERT("received == -1", false);
//     }

//     (*str).resize(length);

//     while (running())
//     {
//         int received = SDLNet_TCP_Recv(socket, str->data(), length*sizeof(char));

//         if (received == length*sizeof(char))
//         {
//             break;
//         }

//         else if (received == -1)
//         {
//             IDK_ASSERT("received == -1", false);
//         }
//     }
// }


// void
// meatnet::Base::string_send( TCPsocket socket, const std::string &str )
// {
//     if (!generic_send(socket, str.length()))
//     {
//         IDK_ASSERT("sent == -1", false);
//     }

//     while (running())
//     {
//         int sent = SDLNet_TCP_Send(socket, str.data(), str.length()*sizeof(char));

//         if (sent == str.length()*sizeof(char))
//         {
//             break;
//         }

//         else if (sent == -1)
//         {
//             IDK_ASSERT("sent == -1", false);
//         }
//     }
// }



// void
// meatnet::Base::_error( TCPsocket socket )
// {
//     std::cout << "[meatnet::Base] Error: \"" << SDLNet_GetError() << "\"\n";

//     if (socket)
//     {
//         SDLNet_TCP_Close(socket);
//     }

//     shutdown();
// }


// void
// meatnet::Base::file_recv( TCPsocket socket, meatnet::File &file )
// {
//     // string_recv(socket, &(file.name));

//     // if (!generic_recv(socket, &(file.nbytes)))
//     // {
//     //     IDK_ASSERT("received == -1", false);
//     // }

//     // void *data = std::malloc(file.nbytes);
//     // std::cout << "[file_recv] Receiving file \"" << file.name << "\"(" << file.nbytes << " b)\n";

//     // while (running())
//     // {
//     //     int received = SDLNet_TCP_Recv(socket, data, file.nbytes);

//     //     if (received == file.nbytes)
//     //     {
//     //         break;
//     //     }

//     //     else if (received == -1)
//     //     {
//     //         IDK_ASSERT("received == -1", false);
//     //     }
//     // }

//     // file.data = data;
//     // std::cout << "[file_recv] Received file\n";
// }



// void
// meatnet::Base::file_send( TCPsocket socket, const std::string &filepath )
// {
//     // size_t nbytes;
//     // void  *data;

//     // std::ifstream stream(filepath, std::ios::binary);
//     // stream.seekg(0, std::ios::end);
//     // nbytes = stream.tellg();
//     // stream.seekg(0, std::ios::beg);

//     // data = std::malloc(nbytes);
//     // stream.read(reinterpret_cast<char *>(data), nbytes);
//     // stream.close();


//     // std::string filename = std::filesystem::path(filepath).filename().string();
//     // std::cout << "[file_send] Sending file \"" << filename << "\" (" << nbytes << " b)\n";
//     // string_send(socket, filename);

//     // if (!generic_send(socket, nbytes))
//     // {
//     //     IDK_ASSERT("sent == -1", false);
//     // }

//     // while (running())
//     // {
//     //     int sent = SDLNet_TCP_Send(socket, data, nbytes);

//     //     if (sent == nbytes)
//     //     {
//     //         break;
//     //     }

//     //     else if (sent == -1)
//     //     {
//     //         IDK_ASSERT("sent == -1", false);
//     //     }
//     // }

//     // std::free(data);
//     // std::cout << "[file_send] Sent file\n";

// }


// bool
// meatnet::Base::msgheader_send( TCPsocket socket, const meatnet::Message &msg, const void *data )
// {
//     std::cout << "[meatnet::Base] Sending Message... \n";

//     while (running())
//     {
//         int n = SDLNet_TCP_Send(socket, (const void *)(&msg), sizeof(Message));
    
//         if (n < 0)               return false;
//         if (n == sizeof(Message)) break;
//     }

//     std::cout << "[meatnet::Base] Sending Message... success\n";

//     return true;
// }


// bool
// meatnet::Base::msgheader_recv( TCPsocket socket, meatnet::Message &msg, void *data )
// {
//     int n = SDLNet_TCP_Recv(socket, (void *)(&msg), sizeof(Message));
//     return (n == sizeof(Message));
// }



// bool
// meatnet::Base::data_send( TCPsocket socket, const meatnet::Message &msg, const void *data )
// {
//     if (msg.nbytes == 0)
//         return true;

//     while (running())
//     {
//         int n = SDLNet_TCP_Send(socket, data, msg.nbytes);
    
//         if (n < 0)               return false;
//         if (n == msg.nbytes)  break;
//     }

//     return true;
// }


// bool
// meatnet::Base::data_recv( TCPsocket socket, meatnet::Message &msg, void *data )
// {
//     if (msg.nbytes == 0)
//         return true;

//     while (running())
//     {
//         int n = SDLNet_TCP_Recv(socket, data, msg.nbytes);
    
//         if (n < 0)               return false;
//         if (n == msg.nbytes)  break;
//     }

//     return true;
// }



// bool
// meatnet::Base::msg_send( TCPsocket socket, const meatnet::Message &msg, const void *data )
// {
//     // Send msg
//     while (running())
//     {
//         int n = SDLNet_TCP_Send(socket, (const void *)(&msg), sizeof(Message));

//         if (n < 0)  return false;
//         if (n == sizeof(Message))  break;
//     }

//     if (msg.nbytes == 0)
//     {
//         return true;
//     }

//     // Send data
//     while (running())
//     {
//         int n = SDLNet_TCP_Send(socket, (const void *)(data), msg.nbytes);

//         if (n < 0)  return false;
//         if (n == msg.nbytes)  break;
//     }

//     return true;
// }



// bool
// meatnet::Base::msg_recv( TCPsocket socket, meatnet::Message &msg, void *data )
// {
//     // Receive msg
//     while (running())
//     {
//         int n = SDLNet_TCP_Recv(socket, (void *)(&msg), sizeof(msg));

//         if (n < 0)  return false;
//         if (n == sizeof(Message))  break;
//     }

//     if (msg.nbytes == 0)
//     {
//         return true;
//     }

//     // Receive data
//     while (running())
//     {
//         int n = SDLNet_TCP_Recv(socket, data, msg.nbytes);

//         if (n < 0)  return false;
//         if (n == msg.nbytes)  break;
//     }

//     return true;
// }





// std::string
// meatnet::req_str( uint32_t req )
// {
//     switch (req)
//     {
//         case REQ_NONE:          return "REQ_NONE";
//         case REQ_CONNECT:       return "REQ_CONNECT";
//         case REQ_DISCONNECT:    return "REQ_DISCONNECT";
//         case REQ_FILE:          return "REQ_FILE";
//         case REQ_JOIN:          return "REQ_JOIN";
//         case REQ_CREATE_BUF:    return "REQ_CREATE_BUF";
//     }

//     return std::to_string(req) + " is not a valid meatnet::Request!!!";
// }


// std::string
// meatnet::res_str( uint32_t res )
// {
//     switch (res)
//     {
//         case RES_NONE:      return "RES_NONE";
//         case RES_OKAY:      return "RES_OKAY";
//         case RES_REFUSE:    return "RES_REFUSE";
//         case RES_STRING:    return "RES_STRING";
//         case RES_FILE:      return "RES_FILE";
//         case RES_PEERDATA:  return "RES_PEERDATA";
//     }

//     return std::to_string(res) + " is not a valid meatnet::Response!!!";
// }
