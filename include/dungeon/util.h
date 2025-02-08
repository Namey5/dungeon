#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdbool.h>
#include <stdint.h>

// Return the greater of two values.
#define Max(a, b) ((a) > (b) ? (a) : (b))
// Return the lesser of two values.
#define Min(a, b) ((a) < (b) ? (a) : (b))
// Clamp value 'x' between 'min' and 'max'.
#define Clamp(x, min, max) Max((min), Min((max), (x)))

// Generates a random float in the range of [0,1].
float Randf32(void);
// Generates a random float in the range of [min,max].
float RandRangef32(float min, float max);
// Generates a random int32_t in the range of [min,max-1].
int32_t RandRangei32(int32_t minInclusive, int32_t maxExclusive);
// Generates a random int32_t index into an array of 'weightCount' size, given 'weights' distribution of probabilities.
// 'totalWeight' is the total sum of all values in 'weights' - if 0 or less, it will be calculated automatically.
int32_t RandIndex(int32_t weightCount, const int32_t weights[], int32_t totalWeight);

int32_t String_Compare_IgnoreCase(int32_t maxSize, const char a[], const char b[]);
#define String_CompareLiteral_IgnoreCase(literal, str) String_Compare_IgnoreCase(sizeof(literal), literal, str)

#endif // __UTIL_H__
