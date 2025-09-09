#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "game.h"
#include "engine.h"
#include "texture.h"

typedef struct renderDataStruct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int width;
    int height;

    float screenSizeRatio;

    Uint64 renderTime;
    Uint64 deltaTime;

    playerStruct player;

    gameTexture textures[TEXTURES_TOTAL];
} renderDataStruct;

void render(renderDataStruct *renderData);
bool set_default_render_data(renderDataStruct *render_data, SDL_Window *window, SDL_Renderer *renderer);
void refresh_render_data_struct(renderDataStruct *renderData);

#endif