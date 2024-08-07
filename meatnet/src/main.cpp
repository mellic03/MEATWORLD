#include <iostream>

#include "meatnet.hpp"


int main( int argc, char **argv )
{
    // // Parse command-line arguments
    // // -----------------------------------------------------------------------------------------
    // if (argc != 5)
    // {
    //     std::cout << "Usage: meatnet -ls SCENE -p PORT\n";
    //     return 0;
    // }


    // std::vector<std::string> args(argc-1);

    // for (int i=1; i<argc; i++)
    // {
    //     args[i-1] = argv[i];
    // }

    // uint16_t port;
    // std::string scene_path;

    // for (int i=0; i<args.size(); i++)
    // {
    //     if (args[i] == "-p" || args[i] == "--port")
    //     {
    //         port = std::stol(args[i+1]);
    //         i += 1;
    //     }

    //     else if (args[i] == "-ls" || args[i] == "--load-scene")
    //     {
    //         scene_path = args[i+1];
    //         i += 1;
    //     }
    // }
    // // -----------------------------------------------------------------------------------------


    meatnet::Host host(4200, 4201);

    // host.loadScene("./scenes/S1.idksc");

    while (host.running())
    {
        // ...
    }

    return 0;
}