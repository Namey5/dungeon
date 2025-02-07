#include "dungeon/dungeon.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dungeon/util.h"

const int32_t roomDistribution[_ROOM_TYPE_COUNT] = {
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
    // ROOM_SPAWN (handled separately):
    0,
};

Dungeon* Dungeon_Create(const int8_t size[2]) {
    assert(size != NULL);

    const int32_t totalRooms = size[0] * size[1];
    assert(totalRooms >= _ROOM_TYPE_COUNT);

    Dungeon *const self = calloc(
        1,
        sizeof(*self)
        // Reserve space at end for rooms:
            + sizeof(self->rooms[0]) * totalRooms
    );
    assert(self != NULL);

    Vec2_Set(self->size, size);

    int32_t totalRoomChance = 0;
    for (int32_t i = 0; i < _ROOM_TYPE_COUNT; ++i) {
        totalRoomChance += roomDistribution[i];
    }

    const RoomType defaultRoom = ROOM_EMPTY;
    {
        // Fill out the rooms linearly based on their distribution chances (rounded down):
        int32_t roomIndex = 0;
        for (RoomType roomType = 0; roomType < _ROOM_TYPE_COUNT; ++roomType) {
            // Make sure each room type appears at least once:
            const int32_t minimumCount = Max(1, roomDistribution[roomType] * totalRooms / totalRoomChance);
            for (int32_t count = 0; count < minimumCount && roomIndex < totalRooms; ++count, ++roomIndex) {
                self->rooms[roomIndex].type = roomType;
            }
        }
        // Fill any remaining rooms with the default;
        for (; roomIndex < totalRooms; ++roomIndex) {
            self->rooms[roomIndex].type = defaultRoom;
        }
    }

    // Randomly shuffle the rooms:
    for (int32_t i = 0; i < totalRooms; ++i) {
        const int32_t rng = RandRangei32(i, totalRooms);
        RoomType current = self->rooms[i].type;
        self->rooms[i].type = self->rooms[rng].type;
        self->rooms[rng].type = current;
    }

    const int8_t invalidPosition[2] = { -1, -1 };
    Vec2_Set(self->spawnPosition, invalidPosition);
    Vec2_Set(self->treasurePosition, invalidPosition);
    for (int8_t position[2] = { 0, 0 }; position[1] < size[1]; ++position[1]) {
        for (position[0] = 0; position[0] < size[0]; ++position[0]) {
            const int32_t index = Dungeon_RoomIndex(self, position);
            Room *const room = &self->rooms[index];
            switch (room->type) {
                case ROOM_EMPTY:
                case ROOM_ITEM:
                case ROOM_PIT:
                case ROOM_TRAP:
                case ROOM_ENEMY: {
                    // TODO: init rooms based on their type.
                } break;
                case ROOM_TREASURE: {
                    assert(Vec2_Equal(self->treasurePosition, invalidPosition));
                    Vec2_Set(self->treasurePosition, position);
                } break;
                case ROOM_SPAWN: {
                    assert(Vec2_Equal(self->spawnPosition, invalidPosition));
                    Vec2_Set(self->spawnPosition, position);
                } break;
                case _ROOM_TYPE_COUNT: {
                    assert(false);
                } break;
            }
        }
    }
    assert(!Vec2_Equal(self->treasurePosition, invalidPosition));
    assert(!Vec2_Equal(self->spawnPosition, invalidPosition));

    return self;
}

void Dungeon_Destroy(Dungeon *const self) {
    assert(self != NULL);
    free(self);
}
