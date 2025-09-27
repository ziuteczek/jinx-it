#ifndef UPDATE_H
#define UPDATE_H

#include <SDL3/SDL.h>

#include "engine.h"
#include "missile.h"

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

void update(inputDataStruct *input_data, struct renderDataStruct *renderData);
void new_missle(misslesStruct *missles_data);
void destroy_all_missles(missleNode *missle_head);
#endif