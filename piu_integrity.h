//
// Created by YthanZhang on 2022/11/15.
//

#ifndef FANV2ST_PIU_INTEGRITY_H
#define FANV2ST_PIU_INTEGRITY_H


#include "stdint.h"

#include "piu_modbus_crc16.h"



#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Create a single byte 2's complement checksum
 * @param start Pointer to the start of an array
 * @param size Size in bytes of the array
 * @return 2's complement checksum
 */
uint8_t piu_checksum8(const uint8_t* start, uint32_t size);
/**
 * @brief Create a 2 bytes 2's complement checksum
 * @note If the provided size % 2 != 0, hidden zeros are added as padding for the
 *      extra byte
 * @param start Pointer to the start of an array
 * @param size Size in bytes of the array
 * @return u16 2's complement checksum
 */
uint16_t piu_checksum16(const uint8_t* start, uint32_t size);
/**
 * @brief Create a 4 bytes 2's complement checksum
 * @note If the provided size % 4 != 0, hidden zeros are added as padding for the
 *      extra bytes
 * @param start Pointer to the start of an array
 * @param size Size in bytes of the array
 * @return u32 2's complement checksum
 */
uint32_t piu_checksum32(const uint8_t* start, uint32_t size);
/**
 * @brief Create a 8 bytes 2's complement checksum
 * @note If the provided size % 8 != 0, hidden zeros are added as padding for the
 *      extra bytes
 * @param start Pointer to the start of an array
 * @param size Size in bytes of the array
 * @return u64 2's complement checksum
 */
uint64_t piu_checksum64(const uint8_t* start, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif    // FANV2ST_PIU_INTEGRITY_H
