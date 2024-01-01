#include <libidk/IDKcommon/idk_assert.hpp>
#include <IDKengine/IDKengine.hpp>
#include <IDKengine/IDKmodules/idk_game.hpp>

#include <string>


class TaxEvasion: public idk::Game
{
private:


public:

    TaxEvasion( std::string name ): Game(name) {  };

    virtual void config() override;
    virtual void registerModules ( idk::Engine & ) override;
    virtual void setup( idk::Engine &, idk::RenderEngine & ) override;
    virtual void mainloop( idk::Engine &, idk::RenderEngine & ) override;

};

