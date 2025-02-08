#include "dungeon/player.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "dungeon/util.h"

const vec2 directions[_ORIENTATION_COUNT] = {
    // ORIENTATION_NORTH:
    {  0,  1 },
    // ORIENTATION_EAST:
    {  1,  0 },
    // ORIENTATION_SOUTH:
    {  0, -1 },
    // ORIENTATION_WEST:
    { -1,  0 },
};

static inline void Player_GetDirection(const Player *const self, vec2 outDirection) {
    outDirection[0] = self->position.current[0] - self->position.previous[0];
    outDirection[1] = self->position.current[1] - self->position.previous[1];
}

Orientation Player_GetOrientation(const Player *const self) {
    assert(self != NULL);

    vec2 direction;
    Player_GetDirection(self, direction);

    for (Orientation orientation = 0; orientation < _ORIENTATION_COUNT; ++orientation) {
        if (Vec2_Equal(direction, directions[orientation])) {
            return orientation;
        }
    }

    assert(false);
    return ORIENTATION_NORTH;
}

void Player_Move(Player *const self, const vec2 direction) {
    assert(self != NULL);

    int8_t currentDirection[2];
    Player_GetDirection(self, currentDirection);

    Vec2_Set(self->position.previous, self->position.current);
    self->position.current[0] += direction[0] * currentDirection[1] + direction[1] * currentDirection[0];
    self->position.current[1] += direction[1] * currentDirection[1] - direction[0] * currentDirection[0];
}

void Player_AdjustHealth(Player *const self, const int8_t amount) {
    assert(self != NULL);
    self->health.current = Clamp(self->health.current + amount, 0, self->health.max);
}
