#include <libidk/idk_assert.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>

#include <string>


class MeatWorldGame: public idk::Game
{
private:

public:
    virtual void registerModules ( idk::EngineAPI& ) final;
    virtual void setup           ( idk::EngineAPI& ) final;
    virtual void mainloop        ( idk::EngineAPI& ) final;
    virtual void shutdown        (                 ) final;

};

