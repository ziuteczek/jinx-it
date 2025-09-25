#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "game.h"
#include "engine.h"
#include "texture.h"

/**
 * Structure containing all data required to render a single frame.
 *
 * Fields:
 * - window:           The SDL window associated with the frame.
 * - renderer:         The SDL renderer used for drawing.
 *
 * - width:            Width of the window in pixels.
 * - height:           Height of the window in pixels.
 *
 * - screenSizeRatio:  Ratio of current screen size to the game's base resolution
 *                     (see BASIC_WIDTH and BASIC_HEIGHT).
 *
 * - renderTime:       Timestamp of the current frame, in milliseconds since the game started.
 * - deltaTime:        Time elapsed since the previous frame, in milliseconds.
 *
 * - player:           Current player data (see playerStruct).
 * - textures:         Array of game textures used for rendering (see gameTexture).
 */
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
void free_textures(renderDataStruct *render_data);
renderDataStruct get_default_render_data(SDL_Window *window, SDL_Renderer *renderer);
void refresh_render_data_struct(renderDataStruct *renderData);

#endif