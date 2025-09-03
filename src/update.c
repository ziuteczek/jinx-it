#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include "update.h"
#include "engine.h"
#include "render.h"

void update(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData)
{
    Uint64 oldFrameRenderTime = renderData->renderTime;

    renderData->renderTime = SDL_GetTicks();
    renderData->deltaTime = renderData->renderTime - oldFrameRenderTime;
}
