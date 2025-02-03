#include "dungeon/dungeon.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dungeon/util.h"

const int32_t roomChances[_ROOM_TYPE_COUNT] = {
    // ROOM_EMPTY:
    50,
    // ROOM_ITEM:
    25,
    // ROOM_PIT:
    10,
    // ROOM_TRAP:
    10,
    // ROOM_ENEMY:
    15,
    // ROOM_TREASURE (handled separately):
    0,
};

Dungeon* Dungeon_Create(const int32_t size[2]) {
    assert(size != NULL);

    Dungeon *const self = calloc(
        1,
        sizeof(*self)
        // Reserve space at end for rooms:
            + sizeof(self->rooms[0]) * (size[0] * size[1])
    );
    assert(self != NULL);

    memcpy(self->size, size, sizeof(int32_t[2]));

    int32_t totalRoomChance = 0;
    for (int32_t i = 0; i < _ROOM_TYPE_COUNT; ++i) {
        totalRoomChance += roomChances[i];
    }

    for (int32_t y = 0; y < size[1]; ++y) {
        for (int32_t x = 0; x < size[0]; ++x) {
            Room *const room = &self->rooms[y * size[0] + x];
            room->type = (RoomType)RandIndex(
                _ROOM_TYPE_COUNT,
                roomChances,
                totalRoomChance
            );
        }
    }

    const int32_t treasurePosition[2] = {
        RandRangei32(0, size[0]),
        RandRangei32(0, size[1]),
    };
    self->rooms[treasurePosition[1] * size[0] + treasurePosition[0]].type = ROOM_TREASURE;

    return self;
}

void Dungeon_Destroy(Dungeon *const self) {
    assert(self != NULL);
    free(self);
}
