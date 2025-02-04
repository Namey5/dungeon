#include "dungeon/util.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

float Randf32(void) {
    const float rng = (float)rand() / (float)RAND_MAX;
    return Clamp(rng, 0.0f, 1.0f);
}

float RandRangef32(const float min, const float max) {
    const float rng = (Randf32() * (max - min)) + min;
    return Clamp(rng, min, max);
}

int32_t RandRangei32(const int32_t minInclusive, const int32_t maxExclusive) {
    const int32_t rng = (int32_t)(Randf32() * (maxExclusive - minInclusive)) + minInclusive;
    return Clamp(rng, minInclusive, maxExclusive - 1);
}

int32_t RandIndex(
    const int32_t weightCount,
    const int32_t weights[weightCount],
    int32_t totalWeight
) {
    if (totalWeight <= 0) {
        totalWeight = 0;
        for (int32_t i = 0; i < weightCount; ++i) {
            totalWeight += weights[i];
        }
    }

    int32_t rng = RandRangei32(0, totalWeight);
    int32_t index;
    for (index = 0; rng >= 0 && index < weightCount; ++index) {
        rng -= weights[index];
    }
    return index - 1;
}

int32_t String_Compare_IgnoreCase(const int32_t maxSize, const char a[maxSize], const char b[maxSize]) {
    assert(a != NULL);
    assert(b != NULL);
    int32_t diff = 0;
    for (int32_t i = 0; diff == 0 && i < maxSize; ++i) {
        diff = tolower((int32_t)a[i]) - tolower((int32_t)b[i]);
    }
    return diff;
}
