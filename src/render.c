#include <SDL3/SDL.h>
#include <stdbool.h>

#include "debug.h"
#include "render.h"
#include "engine.h"
#include "config.h"

void render(renderDataStruct *renderData)
{
    SDL_Renderer *renderer = renderData->renderer;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Drawing map
    SDL_FRect map_render_pos = {
        0,
        0,
        (float)renderData->textures[TEXTURE_MAP].w * renderData->screenSizeRatio,
        (float)renderData->textures[TEXTURE_MAP].h * renderData->screenSizeRatio,
    };
    SDL_RenderTexture(renderer, renderData->textures[TEXTURE_MAP].data, NULL, &map_render_pos);

    // Drawing player
    SDL_FRect player_render_pos = {renderData->player.x, renderData->player.y, renderData->screenSizeRatio * (float)renderData->textures[TEXTURE_PLAYER].w, renderData->screenSizeRatio * (float)renderData->textures[TEXTURE_PLAYER].h};

    SDL_RenderTexture(renderer, renderData->textures[TEXTURE_PLAYER].data, NULL, &player_render_pos);

    SDL_RenderPresent(renderer);
}
void refresh_render_data_struct(renderDataStruct *renderData)
{
}
renderDataStruct get_default_render_data(SDL_Window *window, SDL_Renderer *renderer)
{
    renderDataStruct renderData;
    renderData.renderer = renderer;
    renderData.window = window;
    renderData.exit = false;

    renderData.player.x = 0;
    renderData.player.y = 0;
    renderData.deltaTime = 0;
    renderData.renderTime = 0;

    renderData.player.speed = 750;

    SDL_GetWindowSize(window, &renderData.width, &renderData.height);

    renderData.screenSizeRatio = ((float)renderData.height / (float)BASIC_HEIGHT + (float)renderData.width / (float)BASIC_WIDTH) / 2.0f;

    return renderData;
}