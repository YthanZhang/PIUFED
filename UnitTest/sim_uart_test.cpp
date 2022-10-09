//
// Created by YthanZhang on 2021/12/30.
//


#include "piu_sim_uart.h"

#include "catch2/catch_all.hpp"

#include <iostream>


bool tx = true;

uint16_t rxVal = 0b1111110000;
uint16_t txVal = 0b1111110000;


void setTxBit(bool txVal) { tx = txVal; }


piu_SimUART simUART;


TEST_CASE("Sim UART Test", "[sim_uart]")
{
    piu_SimUART_construct(&simUART, setTxBit);

    /** Test Tx ***************************************************************/
    piu_SimUART_sendTx(&simUART, txVal & 0xFF);
    REQUIRE(tx);

    piu_SimUART_halfDuplexTIMUpdate(&simUART, true);
    REQUIRE(!tx);

    while (piu_SimUART_halfDuplexTIMUpdate(&simUART, true))
    {
        REQUIRE(tx == (txVal & 0x01));
        txVal = txVal >> 1;
    }
    txVal = 0b1110010011;
    REQUIRE(tx);

    /** Test Rx ***************************************************************/
    piu_SimUART_GPIOUpdate(&simUART, false);

    while (!simUART.flag_rxComplete)
    {
        piu_SimUART_halfDuplexTIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;
    }

    REQUIRE(piu_SimUART_getRx(&simUART) == 0xF0);

    /** Test simultaneous Rx Tx on half duplex ********************************/
    piu_SimUART_GPIOUpdate(&simUART, false);
    rxVal = 0b1101010101;

    // tx should be pending be not started
    piu_SimUART_sendTx(&simUART, txVal & 0xFF);
    REQUIRE(tx);

    while (!piu_SimUART_getRxComplete(&simUART))
    {
        piu_SimUART_halfDuplexTIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;

        REQUIRE(tx);    // tx should not change
    }

    piu_SimUART_halfDuplexTIMUpdate(&simUART, false);
    REQUIRE(tx == 0);

    while (piu_SimUART_halfDuplexTIMUpdate(&simUART, false))
    {
        REQUIRE(tx == (txVal & 0x01));
        txVal = txVal >> 1;
    }

    REQUIRE(piu_SimUART_getRx(&simUART) == 0b01010101);

    /** Test simultaneous Rx Tx on full duplex ********************************/
    rxVal = 0b1101010101;
    txVal = 0b1110010011;
    piu_SimUART_GPIOUpdate(&simUART, false);
    piu_SimUART_sendTx(&simUART, txVal & 0xFF);

    piu_SimUART_txTIMUpdate(&simUART);
    REQUIRE_FALSE(tx);

    int i = 0;
    while (!piu_SimUART_getRxComplete(&simUART))
    {
        piu_SimUART_rxTIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;

        ++i;
        if (i % 2)
        {
            piu_SimUART_txTIMUpdate(&simUART);
            REQUIRE(tx == (txVal & 0x01));
            txVal >>= 1;
        }
    }

    while (!piu_SimUART_getTxComplete(&simUART))
    {
        piu_SimUART_txTIMUpdate(&simUART);
        REQUIRE(tx == (txVal & 0x01));
        txVal >>= 1;
    }
    
    rxVal = 0b1101010101;
    REQUIRE(piu_SimUART_getRx(&simUART) == (rxVal & 0xFF));
}
