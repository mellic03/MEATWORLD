#include "idkgame.hpp"


extern "C"
{
    #ifndef EXPORT_API
        #define EXPORT_API __attribute__ ((visibility("default")))
    #endif

    EXPORT_API idk::Game *getInstance()
    {
        return new TaxEvasion("Income Tax Evasion");
    }
}
