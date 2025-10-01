#ifndef MISSILE_H
#define MISSILE_H

#include <SDL3/SDL.h>
#include "texture.h"

typedef enum misslesTypes
{
    MISSLE_ROCKET,
    MISSLES_TOTAL
} misslesTypes;

/**
 * Structure constaining information about a missle
 * 
 * pos: Current position of the missle
 * destination: Target position of the missle
 * speed: Speed of the missle in px per second
 * angle: Angle of the missle in degrees
 * texture: Texture of the missle (see texture.h)
 */
typedef struct missleStruct
{
    SDL_FPoint pos;
    SDL_FPoint destination;

    // Speed of the missle in px per second
    int speed;
    float angle;

    textures texture;
} missleStruct;

/**
 * A node in a linked list of missles
 */
typedef struct missleNode
{
    missleStruct data;
    struct missleNode *next;
} missleNode;

/**
 * Structure containing all information about missiles in the game
 * 
 * count: Number of missles in the list
 * last_missle_added: Time when the last missle was added (in milliseconds)
 * data: Pointer to the first node in the list
 */
typedef struct misslesStruct
{
    int count;
    Uint64 last_missle_added;

    missleNode *data;
} misslesStruct;

#endif