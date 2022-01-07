//
// Created by YthanZhang on 2022/1/6.
//

#include "piu_modbus_crc16.h"


uint16_t piu_modbus_crc16(const uint8_t* data, uint8_t dataLength)
{
    uint16_t crc = 0xFFFF;    // Initialized CRC with value 0xFFFF
    for (uint8_t i = 0; i < dataLength; ++i)
    {
        crc ^= data[i];    // XOR crc with data

        for (uint8_t j = 0; j < 8; ++j)
        {
            if (crc & 0x0001)    // if last bit is not 0
            {
                crc = crc >> 1;
                crc ^= 0xA001;    // XOR with reflected polynomial 0x8005
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

