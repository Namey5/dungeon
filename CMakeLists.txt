cmake_minimum_required(VERSION 3.25)

project(dungeon LANGUAGES C)

add_executable(dungeon)
set_target_properties(
    dungeon PROPERTIES
    C_STANDARD 11
)
target_compile_options(
    dungeon PRIVATE
    $<IF:$<C_COMPILER_ID:MSVC>,
        /WX /W4,
        -Werror -Wall -Wextra -Wpedantic>
)
target_sources(
    dungeon
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/dungeon/dungeon.h
                include/dungeon/item.h
                include/dungeon/player.h
                include/dungeon/util.h
    PRIVATE
        src/main.c
        src/dungeon.c
        src/player.c
        src/util.c
)
