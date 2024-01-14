#include <libidk/idk_assert.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>

#include <string>


class IDKGame: public idk::Game
{
private:

public:

    IDKGame( const std::string &name ): Game(name) {  };

    virtual void config() final;
    virtual void registerModules ( idk::EngineAPI & ) final;
    virtual void setup           ( idk::EngineAPI & ) final;
    virtual void mainloop        ( idk::EngineAPI & ) final;

};

