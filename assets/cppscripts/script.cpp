#include <libidk/idk_platform.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include <iostream>


IDK_EXPORT_API
(
    int idk_scriptentry( idk::EngineAPI & );
)


int idk_scriptentry( idk::EngineAPI &api )
{
    idk::Engine &engine = api.getEngine();

    std::cout << "Hello from script!\n";
    std::cout << "engine.deltaTime(): " << engine.deltaTime() << "\n";

    return 0;
}



