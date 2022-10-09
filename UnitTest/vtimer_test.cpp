//
// Created by YthanZhang on 2021/12/24.
//

#include "piu_vtimer.h"

#include "catch2/catch_all.hpp"


static bool flag_callback = false;
void callback() { flag_callback = true; }


TEST_CASE("Virtual timer test", "[vtimer]")
{
    piu_VTimer vtimer;
    piu_VTimer_construct(&vtimer, 3, piu_VTMode_OneShot, nullptr);

    SECTION("basic oneshot overflow test")
    {
        piu_VTimer_startCounter(&vtimer);

        piu_VTimer_tick(&vtimer);    // 1
        REQUIRE(vtimer.counter == 1);

        piu_VTimer_tick(&vtimer);    // 2
        piu_VTimer_tick(&vtimer);    // 3
        REQUIRE(vtimer.counter == 3);

        piu_VTimer_tick(&vtimer);    // overflow
        REQUIRE(vtimer.counter == 0);
        REQUIRE(piu_VTimer_getOverflow(&vtimer));
        REQUIRE_FALSE(piu_VTimer_getOverOverflow(&vtimer));
        REQUIRE_FALSE(piu_VTimer_getCounterActive(&vtimer));

        piu_VTimer_tick(&vtimer);    // oneshot should not tick
        REQUIRE(vtimer.counter == 0);
    }

    SECTION("basic continuous overflow test")
    {
        piu_VTimer_startCounter(&vtimer);
        piu_VTimer_setTimerMode(&vtimer, piu_VTMode_Continuous);

        piu_VTimer_tick(&vtimer);    // 1
        REQUIRE(vtimer.counter == 1);

        piu_VTimer_tick(&vtimer);    // 2
        piu_VTimer_tick(&vtimer);    // 3
        REQUIRE(vtimer.counter == 3);

        piu_VTimer_tick(&vtimer);    // overflow
        REQUIRE(vtimer.counter == 0);
        REQUIRE(piu_VTimer_getOverflow(&vtimer));
        REQUIRE_FALSE(piu_VTimer_getOverOverflow(&vtimer));
        REQUIRE(piu_VTimer_getCounterActive(&vtimer));

        piu_VTimer_tick(&vtimer);    // continuous should tick
        REQUIRE(vtimer.counter == 1);
    }
    
    SECTION("over overflow test")
    {
        piu_VTimer_startCounter(&vtimer);
        piu_VTimer_setTimerMode(&vtimer, piu_VTMode_Continuous);

        for(uint8_t i = 0; i < 8; ++i)
        {
            piu_VTimer_tick(&vtimer);
        }
        
        REQUIRE(piu_VTimer_getOverflow(&vtimer));
        REQUIRE(piu_VTimer_getOverOverflow(&vtimer));
        REQUIRE(piu_VTimer_getCounter(&vtimer) == 0);
    }
    
    SECTION("callback test")
    {
        piu_VTimer_setCallback(&vtimer, callback);
        piu_VTimer_startCounter(&vtimer);
        
        REQUIRE_FALSE(flag_callback);
        
        for(uint8_t i = 0; i < 4; ++i)
        {
            piu_VTimer_tick(&vtimer);
        }
        
        REQUIRE(flag_callback);
    }
}
