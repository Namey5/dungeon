#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include <stdbool.h>
#include <stdint.h>

#include "dungeon/item.h"
#include "dungeon/vec2.h"

typedef struct Dungeon Dungeon;
typedef struct Room Room;

typedef enum RoomType {
    ROOM_EMPTY,
    ROOM_ITEM,
    ROOM_PIT,
    ROOM_TRAP,
    ROOM_ENEMY,
    ROOM_TREASURE,
    ROOM_SPAWN,
    _ROOM_TYPE_COUNT,
} RoomType;

static inline const char* RoomType_ToString(const RoomType self) {
    switch (self) {
        case ROOM_EMPTY: return "EMPTY";
        case ROOM_ITEM: return "ITEM";
        case ROOM_PIT: return "PIT";
        case ROOM_TRAP: return "TRAP";
        case ROOM_ENEMY: return "ENEMY";
        case ROOM_TREASURE: return "TREASURE";
        case ROOM_SPAWN: return "SPAWN";
        case _ROOM_TYPE_COUNT: return "[UNKNOWN]";
    }
    return "[ERROR]";
}

struct Room {
    RoomType type;
    bool visited;
    union {
        uint8_t empty;
        ItemType item;
        uint8_t pit;
        struct {
            int8_t maxDamage;
        } trap;
        struct {
            int8_t health;
            int8_t maxDamage;
        } enemy;
        uint8_t treasure;
        uint8_t spawn;
    };
};

void Room_InitEmpty(Room* self);
void Room_InitItem(Room* self);
void Room_InitPit(Room* self);
void Room_InitTrap(Room* self);
void Room_InitEnemy(Room* self);
void Room_InitTreasure(Room* self);
void Room_InitSpawn(Room* self);
void Room_Clear(Room* self);

struct Dungeon {
    vec2 size;
    vec2 spawnPosition;
    vec2 treasurePosition;
    Room* rooms;
};

Dungeon* Dungeon_Create(const vec2 size);
void Dungeon_Destroy(Dungeon* self);

static inline int32_t Dungeon_RoomIndex(const Dungeon *const self, const vec2 position) {
    return position[1] * self->size[0] + position[0];
}

#endif // __DUNGEON_H__
