cmake_minimum_required(VERSION 3.25)

project(
    dungeon
    VERSION 0.1.0
    LANGUAGES C
)

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
target_compile_definitions(
    dungeon PRIVATE
    # MSVC complains about scanf usage
    _CRT_SECURE_NO_WARNINGS
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
                include/dungeon/vec2.h
    PRIVATE
        src/main.c
        src/dungeon.c
        src/player.c
        src/util.c
)
