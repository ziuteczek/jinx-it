#ifndef RENDER_H
#define RENDER_H
#include <SDL3/SDL.h>
#include <stdbool.h>
#include "game.h"

typedef struct renderDataStruct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int width;
    int height;

    Uint64 renderTime;
    Uint64 deltaTime;

    playerStruct player;

    bool exit;
} renderDataStruct;

void render(renderDataStruct *renderData);
renderDataStruct get_default_render_data(SDL_Window *window, SDL_Renderer *renderer);
void refreshRenderDataStruct(renderDataStruct *renderData);

#endif