#ifndef MISSILE_H
#define MISSILE_H

#include <SDL3/SDL.h>
#include "texture.h"

typedef enum misslesTypes
{
    MISSLE_ROCKET,
    MISSLES_TOTAL
} misslesTypes;

typedef struct missleStruct
{
    SDL_FPoint pos;
    SDL_FPoint destination;

    // Speed of the missle in px per second
    int speed;
    float angle;

    textures texture;
} missleStruct;

typedef struct missleNode
{
    missleStruct data;
    struct missleNode *next;
} missleNode;

typedef struct misslesStruct
{
    int count;
    Uint64 last_missle_added;

    missleNode *data;
} misslesStruct;

#endif