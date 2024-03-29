//
// Created by YthanZhang on 2022/9/16.
//

#ifndef FANV2ST_PIU_NUMBER_H
#define FANV2ST_PIU_NUMBER_H


#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

#define u8_MAX UINT8_MAX
#define u8_MIN 0

#define i8_MAX INT8_MAX
#define i8_MIN INT8_MIN

#define u16_MAX UINT16_MAX
#define u16_MIN 0

#define i16_MAX INT16_MAX
#define i16_MIN INT16_MIN

#define u32_MAX UINT32_MAX
#define u32_MIN 0

#define i32_MAX INT32_MAX
#define i32_MIN INT32_MIN

#define u64_MAX UINT64_MAX
#define u64_MIN 0

#define i64_MAX INT64_MAX
#define i64_MIN INT64_MIN


typedef float f32;
typedef double f64;


#ifdef __cplusplus
}
#endif

#endif    // FANV2ST_PIU_NUMBER_H
