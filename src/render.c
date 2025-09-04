#include <SDL3/SDL.h>
#include <stdbool.h>

#include "debug.h"
#include "render.h"

void render(renderDataStruct *renderData)
{
    SDL_SetRenderDrawColor(renderData->renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderData->renderer);

    SDL_SetRenderDrawColor(renderData->renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_FRect x = {renderData->player.x, renderData->player.y, 50, 50};
    SDL_RenderRect(renderData->renderer, &x);

    SDL_RenderPresent(renderData->renderer);
}
void refreshRenderDataStruct(renderDataStruct *renderData)
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

    return renderData;
}