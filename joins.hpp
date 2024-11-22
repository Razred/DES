#pragma once

#include <stdio.h>
#include <stdint.h>

uint64_t join32To64(uint32_t block32_1, uint32_t block32_2)
{
   uint64_t block64;
   block64 = (uint64_t)block32_1;
   block64 = (uint64_t)(block64 << 32) | block32_2;

   return block64;
}

uint64_t join28To56(uint32_t block28_1, uint32_t block28_2)
{
   uint64_t block56;
   block56 = (block28_1 >> 4);
   block56 = ((block56 << 32) | block28_2) << 4;

   return block56;
}

uint64_t join8To64(uint8_t *blocks8)
{
   uint64_t block64;
   for (uint8_t *p = blocks8; p < blocks8 + 8; ++p)
   {
      block64 = (block64 << 8) | *p;
   }

   return block64;
}

uint32_t join4To32(uint8_t *blocks4)
{
   uint32_t block32;
   for (uint8_t *p = blocks4; p < blocks4 + 4; ++p)
   {
      block32 = (block32 << 8) | *p;
   }

   return block32;
}

uint32_t join8To32(uint8_t *blocks8)
{
   uint64_t block32;
   for (uint8_t *p = blocks8; p < blocks8 + 4; ++p)
   {
      block32 = (block32 << 8) | *p;
   }
   return block32;
}