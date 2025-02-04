#include "dungeon/player.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "dungeon/util.h"

Orientation Player_GetOrientation(const Player *const self) {
    assert(self != NULL);

    const int8_t direction[2] = {
        self->position.current[0] - self->position.previous[0],
        self->position.current[1] - self->position.previous[1],
    };

    if (Vec2_Equal(direction, (int8_t[2]) { 0, 1 })) {
        return ORIENTATION_NORTH;
    } else if (Vec2_Equal(direction, (int8_t[2]) { 1, 0 })) {
        return ORIENTATION_EAST;
    } else if (Vec2_Equal(direction, (int8_t[2]) { 0, -1 })) {
        return ORIENTATION_SOUTH;
    } else if (Vec2_Equal(direction, (int8_t[2]) { -1, 0 })) {
        return ORIENTATION_WEST;
    } else {
        assert(false);
    }
}
