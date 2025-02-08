#ifndef __ITEM_H__
#define __ITEM_H__

typedef enum ItemType {
    ITEM_FOOD,
    ITEM_SWORD,
    ITEM_SHIELD,
    ITEM_ROPE,
    ITEM_HOOK,
    ITEM_ROCK,
    _ITEM_TYPE_COUNT,
} ItemType;

static inline const char* ItemType_ToString(const ItemType self) {
    switch (self) {
        case ITEM_FOOD: return "FOOD";
        case ITEM_SWORD: return "SWORD";
        case ITEM_SHIELD: return "SHIELD";
        case ITEM_ROPE: return "ROPE";
        case ITEM_HOOK: return "HOOK";
        case ITEM_ROCK: return "ROCK";
        case _ITEM_TYPE_COUNT: return "[ERROR]";
    }
    return "[ERROR]";
}

#endif // __ITEM_H__
