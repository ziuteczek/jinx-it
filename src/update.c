#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "update.h"
#include "engine.h"
#include "render.h"

void update(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData)
{
    Uint64 oldFrameRenderTime = renderData->renderTime;

    renderData->renderTime = SDL_GetTicks();
    renderData->deltaTime = renderData->renderTime - oldFrameRenderTime;

    // player speed expressed in pixels traveled every milisecond
    const float speedMS = renderData->player.speed / 100;

    // number of pixels traveled by player
    const int pixelsTraveled = round(renderData->deltaTime * speedMS);
}
walkingDirection _getWalkingDirection(keyPressState keyPress)
{
    walkingDirection direction;
}