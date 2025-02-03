#ifndef __UTIL_H__
#define __UTIL_H__

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
// Generates a random int32_t in the range of [min,max].
int32_t RandRangei32(int32_t min, int32_t max);
// Generates a random int32_t index into an array of 'weightCount' size, given 'weights' distribution of probabilities.
// 'totalWeight' is the total sum of all values in 'weights' - if 0 or less, it will be calculated automatically.
int32_t RandIndex(int32_t weightCount, const int32_t weights[weightCount], int32_t totalWeight);

#endif // __UTIL_H__
