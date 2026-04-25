
#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

// ignore endianness
static inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (r) | (g << 8) | (b << 16) | (a << 24);
}

static inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (r) | (g << 8) | (b << 16) | (0xFF << 24);
}

#endif // COLOR_H