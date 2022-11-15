#include "piu_integrity.h"

#include "piu_number.h"


u8 piu_checksum8(const u8* start, u32 size)
{
    u8 checksum = 0;
    for (u32 i = 0; i < size; ++i) { checksum += start[i]; }
    return ~checksum + 1;
}

u16 piu_checksum16(const u8* start, u32 size)
{
    const u32 remained_size = size % 2;
    const u32 aligned_size  = size - remained_size;

    u16 checksum = 0;
    for (u32 i = 0; i < aligned_size; i += 2)
    {
        checksum += (u16)start[i] | ((u16)start[i + 1] << 8);
    }

    // checksum for non-aligned bytes
    for (u32 i = 0; i < remained_size; ++i)
    {
        checksum += (u16)start[aligned_size + i] << (8 * i);
    }
    return ~checksum + 1;
}

u32 piu_checksum32(const u8* start, u32 size)
{
    const u32 remained_size = size % 4;
    const u32 aligned_size  = size - remained_size;

    u32 checksum = 0;
    for (u32 i = 0; i < aligned_size; i += 4)
    {
        checksum += (u32)start[i] | ((u32)start[i + 1] << 8) |
                    ((u32)start[i + 2] << 16) | ((u32)start[i + 3] << 24);
    }

    // checksum for non-aligned bytes
    for (u32 i = 0; i < remained_size; ++i)
    {
        checksum += (u32)start[aligned_size + i] << (8 * i);
    }
    return ~checksum + 1;
}

u64 piu_checksum64(const u8* start, u32 size)
{
    const u32 remained_size = size % 8;
    const u32 aligned_size  = size - remained_size;

    u64 checksum = 0;
    for (u32 i = 0; i < aligned_size; i += 8)
    {
        checksum += (u64)start[i] | ((u64)start[i + 1] << 8) |
                    ((u64)start[i + 2] << 16) | ((u64)start[i + 3] << 24) |
                    ((u64)start[i + 4] << 32) | ((u64)start[i + 5] << 40) |
                    ((u64)start[i + 6] << 48) | ((u64)start[i + 7] << 56);
    }

    // checksum for non-aligned bytes
    for (u32 i = 0; i < remained_size; ++i)
    {
        checksum += (u64)start[aligned_size + i] << (8 * i);
    }
    return ~checksum + 1;
}
