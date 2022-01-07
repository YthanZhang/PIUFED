//
// Created by YthanZhang on 2022/1/6.
//

#ifndef PIU_MODBUS_CRC16_H
#define PIU_MODBUS_CRC16_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


uint16_t piu_modbus_crc16(const uint8_t* data, uint8_t dataLength);


#ifdef __cplusplus
}
#endif

#endif    // PIU_MODBUS_CRC16_H
