#include <libidk/idk_assert.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>

#include "./civillian.hpp"

#include <string>


class IDKGame: public idk::Module
{
private:
    idk::Allocator<idkg::Civillian> m_civillians;


public:

    virtual void    init    ( idk::EngineAPI &api ) { registerModules(api); setup(api); };
    virtual void    deinit  (                     ) {  };

    virtual void    stage_A ( idk::EngineAPI &api ) { mainloop(api); };
    virtual void    stage_B ( idk::EngineAPI &api ) {  };
    virtual void    stage_C ( idk::EngineAPI &api ) {  };


    virtual void config          (                  ) final;
    virtual void registerModules ( idk::EngineAPI & ) final;
    virtual void setup           ( idk::EngineAPI & ) final;
    virtual void mainloop        ( idk::EngineAPI & ) final;

};

