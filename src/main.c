#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dungeon/dungeon.h"

const int32_t defaultDungeonSize[2] = { 10, 10 };

void PrintMap(const Dungeon* dungeon);

int32_t main(const int32_t argc, const char *const argv[argc]) {
    if (argc > 1) {
        printf("Launching with %d arg(s):\n", argc);
        for (int32_t i = 0; i < argc; ++i) {
            printf("|  %s\n", argv[i]);
        }
        printf("\n");
    }

    srand((int32_t)time(NULL));

    Dungeon* dungeon = Dungeon_Create(defaultDungeonSize);

    PrintMap(dungeon);

    Dungeon_Destroy(dungeon);

    return 0;
}

void PrintMap(const Dungeon *const dungeon) {
    for (int32_t y = dungeon->size[1]; y >= -2; --y) {
        for (int32_t x = -2; x <= dungeon->size[0]; ++x) {
            if (x > -2) {
                // x-axis alignment:
                printf(" ");
            }

            if (y < -1) {
                // x-axis ruler:
                if (x >= 0 && x < dungeon->size[0]) {
                    printf("%d", x);
                } else {
                    printf(" ");
                }
            } else if (x < -1) {
                // y-axis ruler:
                if (y >= 0 && y < dungeon->size[1]) {
                    printf("%d", y);
                } else {
                    printf(" ");
                }
            } else if (y < 0 || y >= dungeon->size[1]) {
                // top+bottom border:
                printf("-");
            } else if (x < 0 || x >= dungeon->size[0]) {
                // left+right border:
                printf("|");
            } else {
                // room:
                const Room *const room = &dungeon->rooms[y * dungeon->size[0] + x];
                switch (room->type) {
                    case ROOM_EMPTY: {
                        printf(".");
                    } break;
                    case ROOM_ITEM: {
                        printf("+");
                    } break;
                    case ROOM_PIT: {
                        printf("O");
                    } break;
                    case ROOM_TRAP: {
                        printf("X");
                    } break;
                    case ROOM_ENEMY: {
                        printf("E");
                    } break;
                    case ROOM_TREASURE: {
                        printf("*");
                    } break;
                    case _ROOM_TYPE_COUNT: {
                        assert(false);
                    } break;
                }
            }
        }
        printf("\n");
    }
}
