#include "IDKGame.hpp"
#include <libidk/idk_export.hpp>


IDK_EXPORT_API
(
    idk::Game *getInstance()
    {
        static IDKGame *game = nullptr;
    
        if (game == nullptr)
        {
            game = new IDKGame("Game Template");
        }

        return reinterpret_cast<idk::Game *>(game);
    }
)

