#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include <stdint.h>

typedef struct Dungeon Dungeon;
typedef struct Room Room;

typedef enum RoomType {
    ROOM_EMPTY,
    ROOM_ITEM,
    ROOM_PIT,
    ROOM_TRAP,
    ROOM_ENEMY,
    ROOM_TREASURE,
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
        case _ROOM_TYPE_COUNT: return "[UNKNOWN]";
    }
}

struct Room {
    RoomType type;
    union {
        uint8_t empty;
        uint8_t item;
        uint8_t pit;
        uint8_t trap;
        uint8_t enemy;
        uint8_t treasure;
    };
};

struct Dungeon {
    int32_t size[2];
    Room rooms[];
};

Dungeon* Dungeon_Create(const int32_t size[2]);
void Dungeon_Destroy(Dungeon* self);

#endif // __DUNGEON_H__
