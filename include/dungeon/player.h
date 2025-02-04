#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>

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
}

typedef enum ItemType {
    ITEM_FOOD,
    ITEM_SWORD,
    ITEM_SHIELD,
    ITEM_ROPE,
    ITEM_HOOK,
    ITEM_ROCK,
    _ITEM_TYPE_COUNT,
} ItemType;

static inline const char* ItemType_ToString(const ItemType self) {
    switch (self) {
        case ITEM_FOOD: return "FOOD";
        case ITEM_SWORD: return "SWORD";
        case ITEM_SHIELD: return "SHIELD";
        case ITEM_ROPE: return "ROPE";
        case ITEM_HOOK: return "HOOK";
        case ITEM_ROCK: return "ROCK";
        case _ITEM_TYPE_COUNT: return "[ERROR]";
    }
}

struct Player {
    struct {
        int8_t current[2];
        int8_t previous[2];
    } position;
    struct {
        int8_t current;
        int8_t max;
    } health;
    uint8_t inventory[_ITEM_TYPE_COUNT];
};

Orientation Player_GetOrientation(const Player* self);

#endif // __PLAYER_H__
