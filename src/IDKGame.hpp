#include <libidk/idk_assert.hpp>
#include <IDKGameEngine/IDKengine.hpp>
#include <IDKGameEngine/IDKmodules/idk_game.hpp>

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

