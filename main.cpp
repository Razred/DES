#include "DES.hpp"
#include "AES.hpp"
#include "input_print.hpp"

#include <iostream>
#include <limits>

#define MAX_SIZE 1024
#define AES_KEY_SIZE 16

int main()
{
    char alg_mode;
    std::cout << "Enter the cipher mode: A (AES) or D (DES)" << std::endl;
    std::cin >> alg_mode;
    

    switch (alg_mode)
    {
        case 'A':
        {
            uint8_t encrypted[MAX_SIZE] = {0};
            uint8_t decrypted[MAX_SIZE] = {0};
            uint8_t buffer[MAX_SIZE];
            uint8_t key[AES_KEY_SIZE + 1] = "AES_key_128_bits";

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            size_t length = input_string(buffer);
            print_array(buffer, length);

            length = AES(encrypted, 'E', key, buffer, length);
            print_array(encrypted, length);

            length = AES(decrypted, 'D', key, encrypted, length);
            print_array(decrypted, length);
            break;
        }
        case 'D':
        {
            uint8_t encrypted[MAX_SIZE], decrypted[MAX_SIZE];
            uint8_t buffer[MAX_SIZE] = {0};
            uint8_t key[9] = "RNikitaS";

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            size_t length = input_string(buffer);
            print_array(buffer, length);

            length = DES(encrypted, 'E', key, buffer, length);
            print_array(encrypted, length);

            length = DES(decrypted, 'D', key, encrypted, length);
            print_array(decrypted, length);
            break;
        }
        default:
            std::cout << "NOT CORRECT MODE" << std::endl;
            break;
    }

    return 0;
}