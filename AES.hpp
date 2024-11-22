#pragma once

#include "splits.hpp"
#include "joins.hpp"
#include "AES_keys.hpp"


size_t AES(uint8_t * to, uint8_t mode, uint8_t * key, uint8_t * from, size_t length) {

    const uint8_t key_size = 16;

    length = (length % key_size == 0) ? length : length + (key_size - (length % key_size));

    const uint8_t Nb = 4;
    const uint8_t Nk = key_size / Nb;
    const uint8_t Nr = (key_size == 16) ? 10 : (key_size == 24) ? 12 : 14;
    const size_t Nw = Nb * (Nr + 1);

    uint32_t Wkey[Nw];
    keyExpansion(Wkey, key, Nb, Nk, Nr);

    uint8_t block[16];
    
    switch(mode) {
        case 'E': {
            for (size_t i = 0; i < length; i += 16) {
                copyTranspose(block, from + i);
                add_round_key(block, Wkey);
                for (uint8_t round = 1; round <= Nr; ++round) {
                    subBytes(block);
                    shiftRows(block);
                    if (round < Nr) {
                        mixCol(block);
                    }
                    add_round_key(block, Wkey + (Nb * round));
                }
                copyTranspose(to + i, block);
            }
        }
        break;
        case 'D': {
            for (size_t i = 0; i < length; i += 16) {
                copyTranspose(block, from + i);
                add_round_key(block, Wkey + (Nb * Nr));
                for (int8_t round = Nr - 1; round >= 0; --round) {
                    invShiftRows(block);
                    invSubBytes(block);
                    add_round_key(block, Wkey + (Nb * round));
                    if (round > 0) {
                        invMixCol(block);
                    }
                }
                copyTranspose(to + i, block);
            }
        }
        break;
    }
    
    return length;
}


