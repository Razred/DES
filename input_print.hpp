#pragma once

#include <stdio.h>
#include <stdint.h>
#include <iostream>

static inline void print_array(uint8_t *array, size_t length)
{
    printf("%s\t", array);
    printf("[ ");
    for (size_t i = 0; i < sizeof(array); ++i) {
        printf("%x ", array[i]);
    }
    printf("]\n");
}

static inline size_t input_string(uint8_t *buffer)
{
    size_t position = 0;
    uint8_t ch;
    while ((ch = getchar()) != '\n' && position < 1024 - 1)
        buffer[position++] = ch;
    buffer[position] = '\0';
    return position;
}