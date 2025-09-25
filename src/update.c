#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "update.h"
#include "engine.h"
#include "render.h"

#define SQRT_2 1.41421356237309504880

walkingDirection _get_walking_direction(keyPressState key_press[KEYS_TOTAL]);
bool _any_key_pressed(keyPressState key_press[KEYS_TOTAL]);
void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *render_data);

void update(inputDataStruct *input_data, renderDataStruct *render_data)
{
    playerStruct *player = &render_data->player;

    input_data->mouse_pos.x /= render_data->screenSizeRatio;
    input_data->mouse_pos.y /= render_data->screenSizeRatio;

    if (input_data->resize)
    {
        SDL_GetWindowSize(render_data->window, &render_data->width, &render_data->height);
    }

    if (true)
    {
        const float frameRate = 1000.0f / 240.0f;

        if ((float)render_data->deltaTime < frameRate)
        {
            SDL_Delay(frameRate - (float)render_data->deltaTime);
        }
    }

    Uint64 oldFrameRenderTime = render_data->renderTime;

    render_data->renderTime = SDL_GetTicks();

    render_data->deltaTime = render_data->renderTime - oldFrameRenderTime;

    if (player->move_click.following_mouse_click)
    {
        float speed_ms = player->speed / 1000.0f;

        float pixels_traveled = render_data->deltaTime * speed_ms;
        float part_of_path_traveled = pixels_traveled / player->move_click.player_path_length;

        if (player->x <= player->move_click.direction.x)
        {
            player->x += part_of_path_traveled * player->move_click.distance.x;
            player->y += part_of_path_traveled * player->move_click.distance.y;
        }
        else
        {
            player->move_click.following_mouse_click = false;
        }
    }

    if (input_data->mouse_press[MOUSE_BUTTON_RIGHT] == MOUSE_STATE_DOWN && !player->move_click.following_mouse_click)
    {

        player->move_click.direction.x = input_data->mouse_pos.x;
        player->move_click.direction.y = input_data->mouse_pos.y;

        // Distance bettwen player and designater pixel
        player->move_click.distance.x = abs(player->move_click.direction.x - player->x);
        player->move_click.distance.y = abs(player->move_click.direction.y - player->y);

        player->move_click.player_path_length = sqrt(pow(player->move_click.distance.x, 2) + pow(player->move_click.distance.y, 2));

        player->move_click.following_mouse_click = true;
    }

    if (_any_key_pressed(input_data->key_press) && render_data->deltaTime > 0)
    {
        _handle_key_press(input_data->key_press, render_data);
    }
}

void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *render_data)
{
    walkingDirection playerDirection = _get_walking_direction(key_press);

    // player speed expressed in pixels traveled every millisecond
    const float speed_ms = render_data->player.speed / 1000.0f;

    // number of pixels traveled by player
    float pixels_moved = render_data->deltaTime * speed_ms;

    switch (playerDirection)
    {
    case DIRECTION_RIGHT:
        render_data->player.x += pixels_moved;
        break;
    case DIRECTION_LEFT:
        render_data->player.x -= pixels_moved;
        break;
    case DIRECTION_TOP:
        render_data->player.y -= pixels_moved;
        break;
    case DIRECTION_BOTTOM:
        render_data->player.y += pixels_moved;
        break;
    case DIRECTION_TOP_RIGHT:
        pixels_moved /= SQRT_2;
        render_data->player.y -= pixels_moved;
        render_data->player.x += pixels_moved;
        break;
    case DIRECTION_TOP_LEFT:
        pixels_moved /= SQRT_2;
        render_data->player.y -= pixels_moved;
        render_data->player.x -= pixels_moved;
        break;
    case DIRECTION_BOTTOM_RIGHT:
        pixels_moved /= SQRT_2;
        render_data->player.y += pixels_moved;
        render_data->player.x += pixels_moved;
        break;
    case DIRECTION_BOTTOM_LEFT:
        pixels_moved /= SQRT_2;
        render_data->player.y += pixels_moved;
        render_data->player.x -= pixels_moved;
        break;
    case DIRECTION_NONE:
    default:
        // No movement
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
    bool down = key_press[KEY_ARROW_DOWN] == KEY_STATE_DOWN;
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