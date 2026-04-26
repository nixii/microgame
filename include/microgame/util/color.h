
#ifndef MG_COLOR_H
#define MG_COLOR_H

#include <stdlib.h>
#include <stdint.h>

typedef uint32_t color;

inline static color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r) | (g << 8) | (b << 16) | (a << 24);
}
inline static color rgb(uint8_t r, uint8_t g, uint8_t b) {
    return rgba(r, g, b, 0xFF);
}
inline static color rgb_rand() {
    return rgb(rand() % 256, rand() % 256, rand() % 256);
}
inline static color rgb_mix(color a, color b) {
    return (((a & 0xFEFEFEFE) >> 1) + ((b & 0xFEFEFEFE) >> 1));
}

#endif // MG_COLOR_H