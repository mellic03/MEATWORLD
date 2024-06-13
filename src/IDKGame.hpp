#include <libidk/idk_assert.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>

#include <string>


class IDKGame: public idk::Game
{
private:

public:
    virtual void config          (                  ) final;
    virtual void registerModules ( idk::EngineAPI & ) final;
    virtual void setup           ( idk::EngineAPI & ) final;
    virtual void mainloop        ( idk::EngineAPI & ) final;
    virtual void shutdown        (                  ) final;

};


// class IDKGame: public idk::Module
// {
// private:

// public:

//     virtual void    init    ( idk::EngineAPI &api ) { registerModules(api); setup(api); };
//     virtual void    deinit  (                     ) {  };

//     virtual void    stage_A ( idk::EngineAPI &api ) { mainloop(api); };
//     virtual void    stage_B ( idk::EngineAPI &api ) {  };
//     virtual void    stage_C ( idk::EngineAPI &api ) {  };


//     virtual void config          (                  ) final;
//     virtual void registerModules ( idk::EngineAPI & ) final;
//     virtual void setup           ( idk::EngineAPI & ) final;
//     virtual void mainloop        ( idk::EngineAPI & ) final;

// };

