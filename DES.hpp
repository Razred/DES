#pragma once

#include "splits.hpp"
#include "joins.hpp"
#include "boxes.h"
#include "substitutions.hpp"
#include "DES_keys.hpp"

uint64_t initialPermutation(uint64_t block64)
{
   uint64_t new_block64 = 0;
   for (uint8_t i = 0; i < 64; ++i)
   {
      new_block64 |= ((block64 >> (64 - _IP[i])) & 0x01) << (63 - i);
   }
   return new_block64;
}

uint64_t finalPermutation(uint64_t block64)
{
   uint64_t new_block64 = 0;
   for (uint8_t i = 0; i < 64; ++i)
   {
      new_block64 |= ((block64 >> (64 - _FP[i])) & 0x01) << (63 - i);
   }
   return new_block64;
}

uint32_t permutation(uint32_t block32)
{
   uint32_t new_block32 = 0;
   for (uint8_t i = 0; i < 32; ++i)
   {
      new_block32 |= ((block32 >> (32 - _P[i])) & 0x01) << (31 - i);
   }
   return new_block32;
}

static inline void swap(uint32_t *left, uint32_t *right)
{
   uint32_t temp = *left;
   *left = *right;
   *right = temp;
}

uint64_t expansionPermutation(uint32_t block32)
{
   uint64_t block48 = 0;
   for (uint8_t i = 0; i < 48; ++i)
   {
      block48 |= (uint64_t)((block32 >> (32 - _EP[i])) & 0x01) << (63 - i);
   }
   return block48;
}

uint32_t funcF(uint32_t block32, uint64_t key48)
{
   uint64_t block48 = expansionPermutation(block32);
   block48 ^= key48;
   block32 = substitutions(block48);
   return permutation(block32);
}

void feistelRound(uint32_t *left, uint32_t *right, uint64_t key48)
{
   uint32_t temp = *right;
   *right = funcF(*right, key48) ^ *left;
   *left = temp;
}

void feistel(uint8_t mode, uint32_t *left, uint32_t *right, uint64_t *keys48)
{
   switch (mode)
   {
      case 'E':
      case 'e':
      {
         for (int8_t round = 0; round < 16; ++round)
         {
            feistelRound(left, right, keys48[round]);
         }
         swap(left, right);
         break;
      }
      case 'D':
      case 'd':
      {
         for (int8_t round = 15; round >= 0; --round)
         {
            feistelRound(left, right, keys48[round]);
         }
         swap(left, right);
         break;
      }
   }
}

size_t DES(uint8_t *to, uint8_t mode, uint8_t *keys8, uint8_t *from, size_t length)
{
   length = length % 8 == 0 ? length : length + (8 - (length % 8));

   uint64_t keys48[16] = {0};
   uint32_t left, right;

   keyExpansion(join8To64(keys8), keys48);

   for (size_t i = 0; i < length; i += 8)
   {
      split64To32(initialPermutation(join8To64(from + i)), &left, &right);
      feistel(mode, &left, &right, keys48);
      split64To8(finalPermutation(join32To64(left, right)), (to + i));
   }

   return length;
}