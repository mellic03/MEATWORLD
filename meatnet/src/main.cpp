#include <iostream>

#include "meatnet.hpp"


int main( int argc, char **argv )
{
    std::vector<std::string> args(argc-1);

    for (int i=1; i<argc; i++)
    {
        args[i-1] = argv[i];
    }



    uint16_t    port       = 4200;
    std::string scene_path = "./scenes/S1.idksc";

    for (int i=0; i<args.size(); i++)
    {
        if (args[i] == "-p" || args[i] == "--port")
        {
            port = std::stol(args[i+1]);
            i += 1;
        }

        else if (args[i] == "-ls" || args[i] == "--load-scene")
        {
            scene_path = args[i+1];
            i += 1;
        }
    }


    std::cout << "[meatnet] Loading scene \"" << scene_path << "\"\n";

    meatnet::Host host;
    host.connect(scene_path, port);

    while (host.running())
    {
        // ...
    }

    return 0;
}