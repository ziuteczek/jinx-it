#ifndef UPDATE_H
#define UPDATE_H

#include <SDL3/SDL.h>

#include "engine.h"
#include "render.h"

typedef enum walkingDirection
{
    DIRECTION_NONE,
    DIRECTION_TOP,
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_BOTTOM,
    DIRECTION_TOP_RIGHT,
    DIRECTION_TOP_LEFT,
    DIRECTION_BOTTOM_RIGHT,
    DIRECTION_BOTTOM_LEFT,
} walkingDirection;

void update(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData);
#endif