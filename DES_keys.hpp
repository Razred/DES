#pragma once

#include "joins.hpp"
#include "boxes.h"

#include <stdio.h>
#include <stdint.h>

#define LSHIFT_28(x, L) ((((x) << (L)) | ((x) >> (-(L) & 27))) & (((uint64_t)1 << 32) - 1))

uint64_t keyContractionPermutation(uint64_t block56)
{
   uint64_t block48 = 0;
   for (uint8_t i = 0; i < 48; ++i)
   {
      block48 |= ((block56 >> (64 - _CP[i])) & 0x01) << (63 - i);
   }

   return block48;
}

void keyExpansionTo48(uint32_t block28_1, uint32_t block28_2, uint64_t *keys48)
{
   uint64_t block56;
   uint8_t n;

   for (uint8_t i = 0; i < 16; ++i)
   {
      switch (i)
      {
         case 0:
         case 1:
         case 8:
         case 15: n = 1; break;
         default: n = 2; break;
      }

      block28_1 = LSHIFT_28(block28_1, n);
      block28_2 = LSHIFT_28(block28_2, n);
      block56 = join28To56(block28_1, block28_2);
      keys48[i] = keyContractionPermutation(block56);
   }
}

void keyPermutation56To28(uint64_t block56, uint32_t *block28_1, uint32_t *block28_2)
{
   for (uint8_t i = 0; i < 28; ++i)
   {
      *block28_1 |= ((block56 >> (64 - _K1P[i])) & 0x01) << (31 - i);
      *block28_2 |= ((block56 >> (64 - _K2P[i])) & 0x01) << (31 - i);
   }
}

void keyExpansion(uint64_t key64, uint64_t *keys48)
{
   uint32_t K1 = 0, K2 = 0;
   keyPermutation56To28(key64, &K1, &K2);
   keyExpansionTo48(K1, K2, keys48);
}
