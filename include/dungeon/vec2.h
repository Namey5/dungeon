#ifndef __VEC2_H__
#define __VEC2_H__

#include <stdbool.h>
#include <stdint.h>

typedef int8_t vec2[2];

// Copy int8_t[2] value from 'src' to 'dest'.
static inline void Vec2_Set(vec2 dest, const vec2 src) {
    dest[0] = src[0];
    dest[1] = src[1];
}

// Check if 2 int8_t[2] are equal.
static inline bool Vec2_Equal(const vec2 a, const vec2 b) {
    return a[0] == b[0] && a[1] == b[1];
}

#endif // __VEC2_H__
