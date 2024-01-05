#include <libidk/idk_assert.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>
#include <IDKGameEngine/IDKengine.hpp>

#include <string>


class IDKGame: public idk::Game
{
private:


public:

    IDKGame( std::string name ): Game(name) {  };

    virtual void config() override;
    virtual void registerModules ( idk::EngineAPI & ) override;
    virtual void setup           ( idk::EngineAPI & ) override;
    virtual void mainloop        ( idk::EngineAPI & ) override;

};

