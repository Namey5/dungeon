#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>

#include "dungeon/item.h"
#include "dungeon/vec2.h"

typedef struct Player Player;

typedef enum Orientation {
    ORIENTATION_NORTH,
    ORIENTATION_EAST,
    ORIENTATION_SOUTH,
    ORIENTATION_WEST,
    _ORIENTATION_COUNT,
} Orientation;

static inline const char* Orientation_ToString(const Orientation self) {
    switch (self) {
        case ORIENTATION_NORTH: return "NORTH";
        case ORIENTATION_EAST: return "EAST";
        case ORIENTATION_SOUTH: return "SOUTH";
        case ORIENTATION_WEST: return "WEST";
        case _ORIENTATION_COUNT: return "[ERROR]";
    }
    return "[ERROR]";
}

struct Player {
    struct {
        vec2 current;
        vec2 previous;
    } position;
    struct {
        int8_t current;
        int8_t max;
    } health;
    uint8_t inventory[_ITEM_TYPE_COUNT];
};

Orientation Player_GetOrientation(const Player* self);
void Player_Move(Player* self, const vec2 direction);
void Player_AdjustHealth(Player* self, int8_t amount);

#endif // __PLAYER_H__
