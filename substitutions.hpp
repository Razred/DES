#pragma once

#include "splits.hpp"
#include "joins.hpp"
#include "boxes.h"

#include <stdio.h>
#include <stdint.h>

uint8_t extreme(uint8_t block2)
{
   return ((block2 >> 6) & 0x2) | ((block2 >> 2) & 0x1);
}

uint8_t middle(uint8_t block2)
{
   return (block2 >> 3) & 0xF;
}

void substitution6To4(uint8_t *blocks6, uint8_t *blocks4)
{
   uint8_t block2, block4;

   for (uint8_t i = 0, j = 0; i < 8; i += 2, ++j)
   {
      block2 = extreme(blocks6[i]);
      block4 = middle(blocks6[i]);
      blocks4[j] = _Sbox[i][block2][block4];

      block2 = extreme(blocks6[i + 1]);
      block4 = middle(blocks6[i + 1]);
      blocks4[j] = (blocks4[j] << 4) | _Sbox[i + 1][block2][block4];
   }
}

uint32_t substitutions(uint64_t block48)
{
   uint8_t blocks4[4], blocks6[8] = {0};

   split48To6(block48, blocks6);
   substitution6To4(blocks6, blocks4);

   return join4To32(blocks4);
}
