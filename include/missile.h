#ifndef MISSILE_H
#define MISSILE_H

#include <SDL3/SDL.h>
#include "texture.h"

typedef enum missilesTypes
{
    MISSILE_ROCKET,
    MISSILES_TOTAL
} missilesTypes;

/**
 * Structure constaining information about a missile
 * 
 * pos: Current position of the missile
 * destination: Target position of the missile
 * speed: Speed of the missile in px per second
 * angle: Angle of the missile in degrees
 * texture: Texture of the missile (see texture.h)
 */
typedef struct missileStruct
{
    SDL_FPoint pos;
    SDL_FPoint destination;

    // Speed of the missile in px per second
    int speed;
    float angle;

    double pixels_per_ms_x;
    double pixels_per_ms_y;

    textures texture;
} missileStruct;

/**
 * A node in a linked list of missiles
 */
typedef struct missileNode
{
    missileStruct data;
    struct missileNode *next;
} missileNode;

/**
 * Structure containing all information about missiles in the game
 * 
 * count: Number of missiles in the list
 * last_missile_added: Time when the last missile was added (in milliseconds)
 * data: Pointer to the first node in the list
 */
typedef struct missilesStruct
{
    int count;
    Uint64 last_missile_added;

    missileNode *data;
} missilesStruct;

#endif