//
// Created by YthanZhang on 2021/12/27.
//


#include "piu_margined_linear.h"

#include "catch2/catch_all.hpp"


TEST_CASE("Margined linear function test", "[MarginedLinear]")
{
    auto ml =
        piu_MarginedLinear_make(10, 10, 10, 50, 50, 50, 1000, 5000, 8000, 8000);


    REQUIRE(piu_MarginedLinear_setX(&ml, 0) == 1000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 10) == 1000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 11) > 5000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 50) == 8000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 55) == 8000);

    piu_MarginedLinear_updateInput(&ml, 5, 10, 20, 80, 90, 95);
    piu_MarginedLinear_updateOutput(&ml, 1000, 5000, 8000, 9000);

    REQUIRE(piu_MarginedLinear_setX(&ml, 10) == 5000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 11) == 5000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 6) == 5000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 5) == 1000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 20) == 5000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 21) > 5000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 79) < 8000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 90) == 8000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 95) == 8000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 96) == 9000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 91) == 9000);
    REQUIRE(piu_MarginedLinear_setX(&ml, 90) == 8000);

    SECTION("Real world Testcase")
    {
        ml = piu_MarginedLinear_make(5243,
                                     6553,
                                     6554,
                                     62258,
                                     62258,
                                     62258,
                                     0,
                                     6554,
                                     65535,
                                     65535);

        REQUIRE(piu_MarginedLinear_setX(&ml,
                                        (uint16_t)std::round(
                                            0.1f * (float)UINT16_MAX)) == 6554);
        REQUIRE(piu_MarginedLinear_setX(&ml, 6554) == 6554);
        
        REQUIRE(std::round((float)6553 * 2000.0f / (float)UINT16_MAX) == 200.0f);
    }
}
