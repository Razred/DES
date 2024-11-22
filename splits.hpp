#pragma once

#include <stdio.h>
#include <stdint.h>

void split64To32(uint64_t block64, uint32_t *block32_1, uint32_t *block32_2)
{
   *block32_1 = (uint32_t)(block64 >> 32);
   *block32_2 = (uint32_t)(block64);
}

void split64To8(uint64_t block64, uint8_t *blocks8)
{
   for (size_t i = 0; i < 8; ++i)
   {
      blocks8[i] = (uint8_t)(block64 >> ((7 - i) * 8));
   }
}

void split48To6(uint64_t block48, uint8_t *blocks6)
{
   for (uint8_t i = 0; i < 8; ++i)
   {
      blocks6[i] = (block48 >> (58 - (i * 6))) << 2;
   }
}

void split32To8(uint32_t block32, uint8_t *blocks8)
{
   for (uint8_t i = 0; i < 4; ++i)
   {
      blocks8[i] = (uint8_t)(block32 >> (24 - i * 8));
   }
}