#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dungeon/dungeon.h"
#include "dungeon/item.h"
#include "dungeon/player.h"
#include "dungeon/util.h"
#include "dungeon/vec2.h"

const vec2 defaultDungeonSize = { 10, 10 };

const char commonActionsText[] =
    "Common Actions:\n"
    "| 'exit' - quit\n"
    "| 'help' - show this menu\n"
    "| 'map' - display your current position and orientation,\n"
    "|         as well as a map of previously explored rooms\n"
    "| 'health' - show the amount of HEALTH you have remaining\n"
    "| 'inventory' - display the totals of each item in your INVENTORY\n"
    "| 'food' - consume 1 FOOD to regain HEALTH";

const char movementActionsText[] =
    "Movement Actions:\n"
    "| 'forward' - move into the room you are currently facing\n"
    "| 'back' - turn around and move back into the previous room\n"
    "| 'left' - turn anti-clockwise and move into the next room\n"
    "| 'right' - turn clockwise and move into the next room";

const char pitActionsText[] =
    "Pit Actions:\n"
    "| 'jump' - attempt to jump across the pit, keeping in mind\n"
    "|          that your gear's weight will influence your chances\n"
    "| 'swing' - use 1 ROPE and 1 HOOK to guarrantee safe passage\n"
    "| 'return' - retreat back into the previous room";

const char enemyActionsText[] =
    "Combat Actions:\n"
    "| 'fight' - attack the enemy (your chances will improve with a SWORD)\n"
    "| 'flee' - attempt to escape to the previous room";

#define CheckInput(action, input) (String_CompareLiteral_IgnoreCase(action, input) == 0)

bool IsGameOver(const Dungeon* dungeon, const Player* player);

void HandleRoom_GeneralInput(char input[32], Dungeon* dungeon, Player* player, Room* room);
void HandleRoom_Item(char input[32], Dungeon* dungeon, Player* player, Room* room);
void HandleRoom_Pit(char input[32], Dungeon* dungeon, Player* player, Room* room);
void HandleRoom_Trap(char input[32], Dungeon* dungeon, Player* player, Room* room);
void HandleRoom_Enemy(char input[32], Dungeon* dungeon, Player* player, Room* room);

bool HandleInput_CommonActions(const char* input, Dungeon* dungeon, Player* player);
bool HandleInput_MovementActions(const char* input, Dungeon* dungeon, Player* player);

void PrintMap(const Dungeon* dungeon, const Player* player, bool onlyVisited);

int32_t main(const int32_t argc, const char *const argv[]) {
    if (argc > 1) {
        printf("Launching with %d arg(s):\n", argc);
        for (int32_t i = 0; i < argc; ++i) {
            printf("|  %s\n", argv[i]);
        }
        printf("\n");
    }

    srand((int32_t)time(NULL));

    Dungeon* dungeon = Dungeon_Create(defaultDungeonSize);

    Player player = {
        .position = {
            .current = { dungeon->spawnPosition[0], dungeon->spawnPosition[1] },
            // This is only used for direction, so spawn facing north:
            .previous = { dungeon->spawnPosition[0], dungeon->spawnPosition[1] - 1 },
        },
        .health = {
            .max = 20,
            .current = 20,
        },
    };

    player.inventory[ITEM_FOOD] = 5;
    player.inventory[ITEM_ROPE] = 1;
    player.inventory[ITEM_HOOK] = 1;

    printf(
        "--------------------------\n"
        "Welcome to this dungeon.\n"
        "You are searching this area for a treasure of great importance.\n"
        "In each new room various events will occur and further instructions will appear.\n"
        "\n"
        "%s\n"
        "%s\n",
        commonActionsText,
        movementActionsText
    );

    char input[32];
    while (!IsGameOver(dungeon, &player)) {
        Room *const room = &dungeon->rooms[Dungeon_RoomIndex(dungeon, player.position.current)];

        printf("--------------------------\n");

        if (room->type == ROOM_TREASURE) {
            PrintMap(dungeon, &player, false);
            printf("Congratulations, you have found the treasure!\n");
            break;
        }

        switch (room->type) {
            case ROOM_EMPTY: {
                printf("You come across an empty room.\n");
                HandleRoom_GeneralInput(input, dungeon, &player, room);
            } break;
            case ROOM_SPAWN: {
                printf("You stand at the entrance to the dungeon.\n");
                HandleRoom_GeneralInput(input, dungeon, &player, room);
            } break;
            case ROOM_ITEM: {
                HandleRoom_Item(input, dungeon, &player, room);
            } break;
            case ROOM_PIT: {
                HandleRoom_Pit(input, dungeon, &player, room);
            } break;
            case ROOM_TRAP: {
                HandleRoom_Trap(input, dungeon, &player, room);
            } break;
            case ROOM_ENEMY: {
                HandleRoom_Enemy(input, dungeon, &player, room);
            } break;
            case ROOM_TREASURE:
            case _ROOM_TYPE_COUNT: {
                assert(false);
            } break;
        }

        // Need to mark this after handling the room incase the room is cleared:
        room->visited = true;

        if (player.health.current <= 0) {
            PrintMap(dungeon, &player, false);
            printf("YOU DIED!\n");
            break;
        }
    }

    Dungeon_Destroy(dungeon);

    return 0;
}

bool IsGameOver(const Dungeon *const dungeon, const Player *const player) {
    (void)dungeon;
    assert(player != NULL);
    return player->health.current == 0;
}

void HandleRoom_GeneralInput(char input[32], Dungeon *const dungeon, Player *const player, Room *const room) {
    (void)room;
    assert(dungeon != NULL);
    assert(player != NULL);

    while (!IsGameOver(dungeon, player)) {
        printf(
            "What do you do (type 'help' for a list of actions)?\n"
            "> "
        );

        const int32_t _read = scanf("%31s", input);
        (void)_read;

        if (HandleInput_MovementActions(input, dungeon, player)) {
            break;
        } else if (!HandleInput_CommonActions(input, dungeon, player)) {
            printf("Unrecognised command '%s'.\n", input);
        }
    }
}

void HandleRoom_Item(char input[32], Dungeon *const dungeon, Player *const player, Room *const room) {
    assert(dungeon != NULL);
    assert(player != NULL);
    assert(room != NULL);

    player->inventory[room->item] += 1;
    printf(
        "You found a %s! You now have %hhd.\n",
        ItemType_ToString(room->item),
        player->inventory[room->item]
    );
    Room_Clear(room);
    HandleRoom_GeneralInput(input, dungeon, player, room);
}

void HandleRoom_Pit(char input[32], Dungeon *const dungeon, Player *const player, Room *const room) {
    assert(dungeon != NULL);
    assert(player != NULL);
    assert(room != NULL);

    printf(
        "You come across a seemingly bottomless pit.\n"
        "%s\n",
        pitActionsText
    );

    while (!IsGameOver(dungeon, player)) {
        printf(
            "What do you do (type 'help' for a list of actions)?\n"
            "> "
        );

        const int32_t _read = scanf("%31s", input);
        (void)_read;

        if (CheckInput("help", input)) {
            printf(
                "%s\n"
                "%s\n",
                commonActionsText,
                pitActionsText
            );
        } else if (CheckInput("jump", input)) {
            int32_t successPercentage = 85;
            for (int32_t i = 0; i < _ITEM_TYPE_COUNT; ++i) {
                successPercentage -= player->inventory[i] * 3;
            }

            if (RandRangei32(0, 100) < successPercentage) {
                printf("You successfully jump the pit!\n");
                HandleRoom_GeneralInput(input, dungeon, player, room);
                break;
            } else {
                printf("You fall to your doom in your attempt to clear the pit.\n");
                player->health.current = 0;
                break;
            }
        } else if (CheckInput("swing", input)) {
            if (player->inventory[ITEM_HOOK] > 0 && player->inventory[ITEM_ROPE]) {
                printf("Using your HOOK and ROPE, you swing to safety on the other side of the pit.\n");
                player->inventory[ITEM_HOOK] -= 1;
                player->inventory[ITEM_ROPE] -= 1;
                Room_Clear(room);
                break;
            } else {
                printf("You must have at least 1 ROPE and 1 HOOK in order to swing across.\n");
            }
        } else if (CheckInput("return", input)) {
            printf("You edge back into the room from whence you came.\n");
            Player_Move(player, (vec2) { 0, -1 });
            break;
        } else if (!HandleInput_CommonActions(input, dungeon, player)) {
            printf("Unrecognised command '%s'.\n", input);
        }
    }
}

void HandleRoom_Trap(char input[32], Dungeon *const dungeon, Player *const player, Room *const room) {
    assert(dungeon != NULL);
    assert(player != NULL);
    assert(room != NULL);

    const int8_t damage = (int8_t)RandRangei32(1, room->trap.maxDamage + 1);
    Player_AdjustHealth(player, -damage);
    printf(
        "You step on a trap and lose %hhd HEALTH (%hhd/%hhd remaining).\n",
        damage,
        player->health.current,
        player->health.max
    );

    room->trap.maxDamage -= (int8_t)RandRangei32(1, 3);
    if (room->trap.maxDamage <= 0) {
        printf("The trap is destroyed and will cause you no more harm.\n");
        Room_Clear(room);
    }

    HandleRoom_GeneralInput(input, dungeon, player, room);
}

void HandleRoom_Enemy(char input[32], Dungeon *const dungeon, Player *const player, Room *const room) {
    assert(dungeon != NULL);
    assert(player != NULL);
    assert(room != NULL);

    printf(
        "A vicious cave beast blocks your path.\n"
        "%s\n",
        enemyActionsText
    );

    while (!IsGameOver(dungeon, player)) {
        printf(
            "You (%hhd/%hhd) | VS | Beast (%hhd/\?\?\?)\n",
            player->health.current,
            player->health.max,
            room->enemy.health
        );

        printf(
            "What do you do (type 'help' for a list of actions)?\n"
            "> "
        );

        const int32_t _read = scanf("%31s", input);
        (void)_read;

        if (CheckInput("help", input)) {
            printf(
                "%s\n"
                "%s\n",
                commonActionsText,
                enemyActionsText
            );
        } else if (CheckInput("fight", input)) {
            if (player->inventory[ITEM_SWORD] > 0) {
                const int8_t damage = (int8_t)RandRangei32(3, 6);
                room->enemy.health -= damage;
                printf("You hit the beast with your SWORD and deal %hhd damage.\n", damage);
            } else {
                const int8_t damage = (int8_t)RandRangei32(0, 4);
                room->enemy.health -= damage;
                printf("You hit the beast with your fists and deal %hhd damage.\n", damage);
            }

            if (room->enemy.health <= 0) {
                printf("The beast is defeated!\n");
                Room_Clear(room);
                HandleRoom_GeneralInput(input, dungeon, player, room);
                break;
            }

            if (player->inventory[ITEM_SHIELD] > 0) {
                const int8_t damage = (int8_t)RandRangei32(0, 3);
                Player_AdjustHealth(player, -damage);
                printf("The beast hits your SHIELD and you take %hhd damage.\n", damage);

                if (Randf32() > 0.5f) {
                    printf("Your SHIELD breaks!\n");
                    player->inventory[ITEM_SHIELD] -= 1;
                }
            } else {
                const int8_t damage = (int8_t)RandRangei32(1, room->enemy.maxDamage);
                Player_AdjustHealth(player, -damage);
                printf("The beast hits you and deals %hhd damage.\n", damage);
            }
        } else if (CheckInput("flee", input)) {
            const float rng = Randf32();
            if (rng > 0.5f) {
                if (rng > 0.8f) {
                    printf("You successfully evade the creature without harm.\n");
                } else {
                    const int8_t damage = (int8_t)RandRangei32(1, room->enemy.maxDamage);
                    Player_AdjustHealth(player, -damage);
                    printf(
                        "You successfully evade the creature, "
                        "but lose %hhd HEALTH in the process (%hhd/%hhd remaining).\n",
                        damage,
                        player->health.current,
                        player->health.max
                    );
                }

                Player_Move(player, (vec2) { 0, -1 });
                break;
            } else {
                const int8_t damage = (int8_t)RandRangei32(1, room->enemy.maxDamage);
                Player_AdjustHealth(player, -damage);
                printf("You fail to evade the creature and lose %hhd HEALTH in the process.\n", damage);
            }
        } else if (!HandleInput_CommonActions(input, dungeon, player)) {
            printf("Unrecognised command '%s'.\n", input);
        }
    }
}

bool HandleInput_CommonActions(const char* input, Dungeon *const dungeon, Player *const player) {
    if (CheckInput("exit", input)) {
        player->health.current = 0;
    } else if (CheckInput("help", input)) {
        printf(
            "%s\n"
            "%s\n",
            commonActionsText,
            movementActionsText
        );
    } else if (CheckInput("map", input)) {
        PrintMap(dungeon, player, true);
    } else if (CheckInput("health", input)) {
        printf("Current HEALTH: %hhd/%hhd\n", player->health.current, player->health.max);
    } else if (CheckInput("inventory", input)) {
        printf("INVENTORY: {\n");
        for (ItemType item = 0; item < _ITEM_TYPE_COUNT; ++item) {
            printf("  %s: %hhd,\n", ItemType_ToString(item), player->inventory[item]);
        }
        printf("}\n");
    } else if (CheckInput("food", input)) {
        if (player->inventory[ITEM_FOOD] == 0) {
            printf("You have no FOOD.\n");
        } else if (player->health.current >= player->health.max) {
            printf(
                "You already have max HEALTH (%hhd/%hhd).\n",
                player->health.current,
                player->health.max
            );
        } else {
            const int8_t health = (int8_t)RandRangei32(1, 6);
            Player_AdjustHealth(player, health);
            player->inventory[ITEM_FOOD] -= 1;
            printf(
                "You consume 1 FOOD and regain %hhd HEALTH (%hhd/%hhd).\n",
                health,
                player->health.current,
                player->health.max
            );
        }
    } else {
        return false;
    }

    return true;
}

bool HandleInput_MovementActions(const char* input, Dungeon *const dungeon, Player *const player) {
    vec2 currentPosition, previousPosition;
    Vec2_Set(currentPosition, player->position.current);
    Vec2_Set(previousPosition, player->position.previous);

    const char* message;
    if (CheckInput("forward", input)) {
        message = "You move forward into the next room.";
        Player_Move(player, (vec2) { 0, 1 });
    } else if (CheckInput("back", input)) {
        message = "You edge back into the room from whence you came.";
        Player_Move(player, (vec2) { 0, -1 });
    } else if (CheckInput("left", input)) {
        message = "You turn left into the next room.";
        Player_Move(player, (vec2) { -1, 0 });
    } else if (CheckInput("right", input)) {
        message = "You turn right into the next room.";
        Player_Move(player, (vec2) { 1, 0 });
    } else {
        return false;
    }

    if (
        player->position.current[0] < 0 || player->position.current[0] >= dungeon->size[0]
        || player->position.current[1] < 0 || player->position.current[1] >= dungeon->size[1]
    ) {
        printf("You come upon a solid wall - please choose a new direction.\n");
        Vec2_Set(player->position.current, currentPosition);
        Vec2_Set(player->position.previous, previousPosition);
        return false;
    }

    printf("%s\n", message);
    return true;
}

void PrintMap(const Dungeon *const dungeon, const Player *const player, const bool onlyVisited) {
    printf(
        "Map Legend:\n"
        "| ^ - player (follows orientation)\n"
        "| H - dungeon entrance\n"
        "| . - empty room\n"
        "| + - item pickup\n"
        "| X - trap\n"
        "| O - pit\n"
        "| E - enemy\n"
        "| * - treasure\n"
        "| ? - undiscovered room\n"
    );

    // Print y-axis in reverse:
    for (int8_t y = dungeon->size[1]; y >= -2; --y) {
        for (int8_t x = -2; x <= dungeon->size[0]; ++x) {
            if (x > -2) {
                // x-axis alignment:
                printf(" ");
            }

            if (y < -1) {
                // x-axis ruler:
                if (x >= 0 && x < dungeon->size[0]) {
                    printf("%hhd", x);
                } else {
                    printf(" ");
                }
            } else if (x < -1) {
                // y-axis ruler:
                if (y >= 0 && y < dungeon->size[1]) {
                    printf("%hhd", y);
                } else {
                    printf(" ");
                }
            } else if (y < 0 || y >= dungeon->size[1]) {
                // top+bottom border:
                printf("-");
            } else if (x < 0 || x >= dungeon->size[0]) {
                // left+right border:
                printf("|");
            } else if (Vec2_Equal((vec2) { x, y }, player->position.current)) {
                // player:
                if (player->position.previous[1] < player->position.current[1]) {
                    printf("^");
                } else if (player->position.previous[0] < player->position.current[0]) {
                    printf(">");
                } else if (player->position.previous[1] > player->position.current[1]) {
                    printf("v");
                } else if (player->position.previous[0] > player->position.current[0]) {
                    printf("<");
                } else {
                    assert(false);
                }
            } else {
                // room:
                const Room *const room = &dungeon->rooms[Dungeon_RoomIndex(dungeon, (vec2) { x, y })];
                if (onlyVisited && !room->visited) {
                    printf("?");
                } else switch (room->type) {
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
                    case ROOM_SPAWN: {
                        printf("H");
                    } break;
                    case _ROOM_TYPE_COUNT: {
                        assert(false);
                    } break;
                }
            }
        }
        printf("\n");
    }

    printf(
        "You are at [%hhd, %hhd] facing %s.\n",
        player->position.current[0],
        player->position.current[1],
        Orientation_ToString(Player_GetOrientation(player))
    );
}
