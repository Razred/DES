#pragma once

#include "joins.hpp"
#include "splits.hpp"
#include "boxes.h"

#include <stdio.h>
#include <stdint.h>

void shiftrArray(uint8_t *array, size_t length, size_t shift)
{
   uint8_t temp;
   size_t index;
   while (shift--)
   {
      temp = array[length - 1];
      for (index = length - 1; index > 0; index--)
         array[index] = array[index - 1];
      array[0] = temp;
   }
}

void shiftlArray(uint8_t *array, size_t length, size_t shift)
{
   uint8_t temp;
   size_t index;
   while (shift--)
   {
      temp = array[0];
      index = 1;
      for (; index < length; ++index)
      {
         array[index - 1] = array[index];
      }
      array[index - 1] = temp;
   }
}

uint32_t subWord(uint32_t word)
{
   uint8_t bytes[4];
   split32To8(word, bytes);
   for (uint8_t i = 0; i < 4; ++i)
   {
      bytes[i] = Sbox[bytes[i]];
   }
   return join8To32(bytes);
}

uint32_t rotWord(uint32_t word)
{
   uint8_t bytes[4];
   split32To8(word, bytes);
   shiftlArray(bytes, 4, 1);
   return join8To32(bytes);
}

void copyTranspose(uint8_t *to, uint8_t *from)
{
   for (uint8_t i = 0; i < 4; ++i)
   {
      for (uint8_t j = 0; j < 4; ++j)
      {
         to[i * 4 + j] = from[j * 4 + i];
      }
   }
}

void copyBlock(uint8_t *to, uint8_t *from)
{
   for (uint8_t i = 0; i < 4; ++i)
   {
      for (uint8_t j = 0; j < 4; ++j)
      {
         to[i * 4 + j] = from[i * 4 + j];
      }
   }
}

void invSubBytes(uint8_t *block)
{
   for (uint8_t i = 0; i < 16; ++i)
   {
      block[i] = InvSbox[block[i]];
   }
}

void subBytes(uint8_t *block)
{
   for (uint8_t i = 0; i < 16; ++i)
   {
      block[i] = Sbox[block[i]];
   }
}

void invShiftRows(uint8_t *block)
{
   for (uint8_t i = 1; i < 4; ++i)
   {
      shiftrArray(block + (i * 4), 4, i);
   }
}

void shiftRows(uint8_t *block)
{
   for (uint8_t i = 1; i < 4; ++i)
   {
      shiftlArray(block + (i * 4), 4, i);
   }
}

uint8_t GFMul(uint8_t a, uint8_t b) {
    uint8_t hi_bit_set, p = 0;
    for(uint8_t counter = 0; counter < 8; counter++) {
        if(b & 0x01) {
            p ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if(hi_bit_set == 0x80)  {
            a ^= 0x1B;      
        }
        b >>= 1;
    }
    return p;
}


void invMixCol(uint8_t * block) {
    uint8_t columns[16];
    copyBlock(columns, block);
    for (uint8_t i = 0; i < 4; ++i) {
        block[0*4+i] = GFMul(0x0E, columns[0*4+i]) ^ GFMul(0x0B, columns[1*4+i]) ^ GFMul(0x0D, columns[2*4+i]) ^ GFMul(0x09, columns[3*4+i]);
        block[1*4+i] = GFMul(0x09, columns[0*4+i]) ^ GFMul(0x0E, columns[1*4+i]) ^ GFMul(0x0B, columns[2*4+i]) ^ GFMul(0x0D, columns[3*4+i]);
        block[2*4+i] = GFMul(0x0D, columns[0*4+i]) ^ GFMul(0x09, columns[1*4+i]) ^ GFMul(0x0E, columns[2*4+i]) ^ GFMul(0x0B, columns[3*4+i]);
        block[3*4+i] = GFMul(0x0B, columns[0*4+i]) ^ GFMul(0x0D, columns[1*4+i]) ^ GFMul(0x09, columns[2*4+i]) ^ GFMul(0x0E, columns[3*4+i]);
    }
}

void mixCol(uint8_t * block) {
    uint8_t columns[16];
    copyBlock(columns, block);
    for (uint8_t i = 0; i < 4; ++i) {
        block[0*4+i] = GFMul(0x02, columns[0*4+i]) ^ GFMul(0x03, columns[1*4+i]) ^ columns[2*4+i] ^ columns[3*4+i];
        block[1*4+i] = columns[0*4+i] ^ GFMul(0x02, columns[1*4+i]) ^ GFMul(0x03, columns[2*4+i]) ^ columns[3*4+i];
        block[2*4+i] = columns[0*4+i] ^ columns[1*4+i] ^ GFMul(0x02, columns[2*4+i]) ^ GFMul(0x03, columns[3*4+i]);
        block[3*4+i] = GFMul(0x03, columns[0*4+i]) ^ columns[1*4+i] ^ columns[2*4+i] ^ GFMul(0x02, columns[3*4+i]);
    }
}
