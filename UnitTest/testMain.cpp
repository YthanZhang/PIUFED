//
// Created by YthanZhang on 2021/12/8.
//


#include "piu_button.h"


#include "catch2/catch_all.hpp"


TEST_CASE("Button input test", "[button]")
{
    piu_Button button;
    piu_Button_construct(&button, 5);
    
    REQUIRE(button.stableThreshold == 5);

    REQUIRE(button.isPressed == 0);
    REQUIRE(button.oldPress == 0);
    REQUIRE(button.stableCounter == 0);
    REQUIRE(button.stableState == 0);
    REQUIRE(button.stableLengthCounter == 0);
    
    // Test first tick
    piu_Button_tick(&button);
    REQUIRE(button.stableCounter == 1);
    REQUIRE(button.stableLengthCounter == 1);

    // Test first update state
    piu_Button_updateState(&button, true);
    REQUIRE(button.isPressed == 1);
    REQUIRE(button.oldPress == 0);
    
    // Test multiple tick
    piu_Button_tick(&button);
    piu_Button_tick(&button);
    REQUIRE(button.oldPress == 1);
    REQUIRE(button.stableCounter == 1);
    REQUIRE(button.stableLengthCounter == 1);
    
    // Test state change
    piu_Button_tick(&button);
    piu_Button_tick(&button);
    piu_Button_tick(&button);
    piu_Button_tick(&button);
    REQUIRE(button.stableState == 1);
    REQUIRE(button.stableLengthCounter == 0);
    
    // Test tick after state change
    piu_Button_tick(&button);
    REQUIRE(button.stableLengthCounter == 1);
    piu_Button_tick(&button);
    REQUIRE(button.stableLengthCounter == 2);

    piu_Button_setStableThreshold(&button, 0);

    
    // check multiple state change won't affect old
    piu_Button_updateState(&button, false);
    piu_Button_updateState(&button, false);
    REQUIRE(button.oldPress == 1);
    REQUIRE(button.isPressed == 0);
    REQUIRE(button.pendPress == 0);
    piu_Button_updateState(&button, true);
    REQUIRE(button.isPressed == 0);
    REQUIRE(button.pendPress == 1);
    
    piu_Button_tick(&button);
    REQUIRE(piu_Button_stableState(&button) == 0);

    piu_Button_tick(&button);
    REQUIRE(piu_Button_stableState(&button) == 1);
    
    button.stableLengthCounter = UINT16_MAX - 2;
    button.stableCounter = UINT16_MAX - 2;

    piu_Button_tick(&button);
    piu_Button_tick(&button);
    piu_Button_tick(&button);
    
    piu_Button_updateState(&button, false);
    
    piu_Button_tick(&button);
    
    REQUIRE(button.stableCounter == 0);
    
}


