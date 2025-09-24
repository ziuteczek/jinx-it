#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL3/SDL.h>

typedef enum textures
{
    TEXTURE_MAP,
    TEXTURE_PLAYER,
    TEXTURES_TOTAL,
} textures;

typedef struct gameTexture
{
    SDL_Texture *data;
    int h;
    int w;
    bool success;
} gameTexture;
#endif