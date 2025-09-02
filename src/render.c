#include <SDL3/SDL.h>
#include <stdbool.h>

#include "render.h"

void render(renderDataStruct *renderData)
{
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(renderData->window, &windowWidth, &windowHeight);

    SDL_SetRenderDrawColor(renderData->renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    SDL_RenderClear(renderData->renderer);
    SDL_RenderPresent(renderData->renderer);
}
void refreshRenderDataStruct(renderDataStruct *renderData)
{
    
}
renderDataStruct getDefaultRenderData(SDL_Window *window, SDL_Renderer *renderer)
{
    renderDataStruct renderData;
    renderData.renderer = renderer;
    renderData.window = window;
    renderData.exit = false;

    SDL_GetWindowSize(window, &renderData.width, &renderData.height);

    return renderData;
}