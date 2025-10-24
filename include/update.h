#ifndef UPDATE_H
#define UPDATE_H

#include <SDL3/SDL.h>

#include "engine.h"
#include "missile.h"
#include "game.h"

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

// Missiles logic
void new_missile(missilesStruct *missiles, playerStruct *player);
void destroy_all_missiles(missileNode **missile_head);
void destroy_missile(missileNode *prev_node, missileNode *to_destroy_node);
#endif