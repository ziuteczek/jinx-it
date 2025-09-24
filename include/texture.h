#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL3/SDL.h>

typedef enum textures
{
    TEXTURE_MAP,
    TEXTURE_PLAYER,
    TEXTURES_TOTAL,
} textures;

/**
 * - w: Width of the texture.
 * - h: Height of the texture.
 * - success: Whether the texture was successfully loaded.
 * - data:    The actual texture data.
 */
typedef struct gameTexture
{
    SDL_Texture *data;
    int h;
    int w;
    bool success;
} gameTexture;
#endif