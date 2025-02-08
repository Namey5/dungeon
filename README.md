Simple text-based dungeon-crawler written in C11 using CMake.

This is a remake of a uni assignment my brother did like 15 years ago :)

Only hard build requirement here is CMake >3.25:
```bash
cd dungeon
cmake -B build
cmake --build build
```

Was developed on Linux using:
 - clang: 19.1.7
 - mold: 2.36.0

But in theory should also build using GCC/MSVC and on other OS's.
