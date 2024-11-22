#pragma once

#include "joins.hpp"
#include "boxes.h"
#include "AES_func.hpp"

#include <stdio.h>
#include <stdint.h>

void keyExpansion(uint32_t *Wkey, uint8_t *key, uint8_t Nb, uint8_t Nk, uint8_t Nr)
{
   for (uint8_t i = 0; i < Nk; ++i)
   {
      Wkey[i] = join8To32(key + (4 * i));
   }
   uint32_t T;
   for (size_t i = Nk; i < (Nb * (Nr + 1)); ++i)
   {
      T = Wkey[i - 1];
      if (i % Nk == 0)
      {
         T = subWord(rotWord(T)) ^ Rcon[i / Nk];
      }
      else if (Nk == 8 && (i % Nk) == 4)
      {
         T = subWord(T);
      }
      Wkey[i] = Wkey[i - Nk] ^ T;
   }
}

void add_round_key(uint8_t *block, uint32_t *Wkeys)
{
   uint8_t bytes[4];
   for (uint8_t i = 0; i < 4; ++i)
   {
      split32To8(Wkeys[i], bytes);
      for (uint8_t j = 0; j < 4; ++j)
      {
         block[j * 4 + i] ^= bytes[j];
      }
   }
}