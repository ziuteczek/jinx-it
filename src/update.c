#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "update.h"
#include "engine.h"
#include "render.h"

walkingDirection _get_walking_direction(keyPressState key_press[KEYS_TOTAL]);
bool _any_key_pressed(keyPressState key_press[KEYS_TOTAL]);
void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *renderData);

void update(keyPressState key_press[KEYS_TOTAL], renderDataStruct *renderData)
{
    Uint64 oldFrameRenderTime = renderData->renderTime;

    renderData->renderTime = SDL_GetTicks();
    renderData->deltaTime = renderData->renderTime - oldFrameRenderTime;

    if (false)
    {
        const float frameRate = 1000.0f / 240.0f;

        if ((float)renderData->deltaTime < frameRate)
        {
            SDL_Delay(frameRate - (float)renderData->deltaTime);
        }
    }

    if (_any_key_pressed(key_press))
    {
        _handle_key_press(key_press, renderData);
    }
}

void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *renderData)
{
    walkingDirection playerDirection = _get_walking_direction(key_press);

    // player speed expressed in pixels traveled every milisecond
    const float speed_ms = renderData->player.speed / 1000.0f;

    // number of pixels traveled by player
    float pixels_moved = renderData->deltaTime * speed_ms;

    switch (playerDirection)
    {
    case DIRECTION_RIGHT:
        renderData->player.x += pixels_moved;
        return;
    case DIRECTION_LEFT:
        renderData->player.x -= pixels_moved;
        return;
    case DIRECTION_TOP:
        renderData->player.y -= pixels_moved;
        return;
    case DIRECTION_BOTTOM:
        renderData->player.y += pixels_moved;
        return;
    }

    pixels_moved /= sqrt(2.0f);

    switch (playerDirection)
    {
    case DIRECTION_TOP_RIGHT:
        renderData->player.y -= pixels_moved;
        renderData->player.x += pixels_moved;
        break;
    case DIRECTION_TOP_LEFT:
        renderData->player.y -= pixels_moved;
        renderData->player.x -= pixels_moved;
        break;
    case DIRECTION_BOTTOM_RIGHT:
        renderData->player.y += pixels_moved;
        renderData->player.x += pixels_moved;
        break;
    case DIRECTION_BOTTOM_LEFT:
        renderData->player.y += pixels_moved;
        renderData->player.x -= pixels_moved;
        break;
    }
}
bool _any_key_pressed(keyPressState key_press[KEYS_TOTAL])
{
    for (int i = 0; i < KEYS_TOTAL; i++)
    {
        if (key_press[i] == KEY_STATE_DOWN)
        {
            return true;
        }
    }
    return false;
}
walkingDirection _get_walking_direction(keyPressState key_press[KEYS_TOTAL])
{
    bool up = key_press[KEY_ARROW_UP] == KEY_STATE_DOWN;
    bool down = key_press[KEY_ARROW_UP] == KEY_STATE_DOWN;
    bool left = key_press[KEY_ARROW_LEFT] == KEY_STATE_DOWN;
    bool right = key_press[KEY_ARROW_RIGHT] == KEY_STATE_DOWN;

    if (up && right)
        return DIRECTION_TOP_RIGHT;
    if (up && left)
        return DIRECTION_TOP_LEFT;
    if (down && right)
        return DIRECTION_BOTTOM_RIGHT;
    if (down && left)
        return DIRECTION_BOTTOM_LEFT;
    if (up)
        return DIRECTION_TOP;
    if (down)
        return DIRECTION_BOTTOM;
    if (right)
        return DIRECTION_RIGHT;
    if (left)
        return DIRECTION_LEFT;

    return DIRECTION_NONE;
}