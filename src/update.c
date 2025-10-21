#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "update.h"
#include "engine.h"
#include "render.h"

#define CAP_FRAME_RATE false

#define SQRT_2 1.41421356237309504880

bool _any_key_pressed(keyPressState key_press[KEYS_TOTAL]);

void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *render_data);
void _handle_mouse_movement(inputDataStruct *input_data, renderDataStruct *render_data);

void _frame_timing(renderDataStruct *render_data);
void _cap_frame_rate(renderDataStruct *render_data, int fps);

void update(inputDataStruct *input_data, renderDataStruct *render_data)
{
    playerStruct *player = &render_data->player;

    if (input_data->resize)
    {
        SDL_GetWindowSize(render_data->window, &render_data->width, &render_data->height);
        render_data->screenSizeRatio = get_screen_size_ratio(render_data->width);
    }
    5;

    input_data->mouse_pos.x /= render_data->screenSizeRatio;
    input_data->mouse_pos.y /= render_data->screenSizeRatio;

#if CAP_FRAME_RATE
    _cap_frame_rate(render_data, 240);
#endif

    _frame_timing(render_data);

    _handle_mouse_movement(input_data, render_data);

    if (_any_key_pressed(input_data->key_press) && render_data->deltaTime > 0)
    {
        _handle_key_press(input_data->key_press, render_data);
    }
}

void _frame_timing(renderDataStruct *render_data)
{
    Uint64 oldFrameRenderTime = render_data->renderTime;

    render_data->renderTime = SDL_GetTicks();

    render_data->deltaTime = render_data->renderTime - oldFrameRenderTime;
}

void _cap_frame_rate(renderDataStruct *render_data, int fps)
{
    const float frameRate = 1000.0f / (float)fps;

    if ((float)render_data->deltaTime < frameRate)
    {
        SDL_Delay(frameRate - (float)render_data->deltaTime);
    }
}
/**
 * Checks if player has reached the destination point
 */
bool _click_travel_finished(playerStruct *player)
{
    bool player_direction_x = player->x < player->move_click.direction.x;
    bool player_direction_y = player->y < player->move_click.direction.y;

    switch (player->move_click.move_direction)
    {
    case MOVE_DIRECTION_X_Y:
        return player_direction_x && player_direction_y;
    case MOVE_DIRECTION_X_NEG_Y:
        return player_direction_x && !player_direction_y;
    case MOVE_DIRECTION_NEG_X_NEG_Y:
        return !player_direction_x && !player_direction_y;
    case MOVE_DIRECTION_NEG_X_Y:
        return !player_direction_x && player_direction_y;
    default:
        return !player_direction_x && player_direction_y;
    }
}

void _handle_player_mouse_moving(renderDataStruct *render_data);
void _handle_player_mouse_start_movement(inputDataStruct *input_data, renderDataStruct *render_data);
/**
 * Handles player movement to the clicked pixel
 */
void _handle_mouse_movement(inputDataStruct *input_data, renderDataStruct *render_data)
{
    playerStruct *player = &render_data->player;

    // If player is currently marked as moving, uptdates his position
    if (player->move_click.following_mouse_click)
    {
        _handle_player_mouse_moving(render_data);
    }

    // Cheks if right mouse button was pressed, if so stats player movement to clicked pixel
    if (input_data->mouse_press[MOUSE_BUTTON_RIGHT] == MOUSE_STATE_DOWN)
    {
        _handle_player_mouse_start_movement(input_data, render_data);
    }
}

/**
 * Initializes player movement to the clicked pixel
 */
void _handle_player_mouse_start_movement(inputDataStruct *input_data, renderDataStruct *render_data)
{
    playerStruct *player = &render_data->player;

    player->move_click.direction.x = input_data->mouse_pos.x - render_data->textures[TEXTURE_PLAYER].w / 2;
    player->move_click.direction.y = input_data->mouse_pos.y - render_data->textures[TEXTURE_PLAYER].h / 2;

    // Distance bettwen player and designater pixel
    player->move_click.distance.x = abs(player->move_click.direction.x - player->x);
    player->move_click.distance.y = abs(player->move_click.direction.y - player->y);

    // Is the player going foward (increasing coordinates)
    bool player_direction_x = player->x < player->move_click.direction.x;
    bool player_direction_y = player->y < player->move_click.direction.y;

    if (player_direction_x && player_direction_y)
    {
        player->move_click.move_direction = MOVE_DIRECTION_X_Y;
    }
    else if (player_direction_x && !player_direction_y)
    {
        player->move_click.move_direction = MOVE_DIRECTION_X_NEG_Y;
    }
    else if (!player_direction_x && !player_direction_y)
    {
        player->move_click.move_direction = MOVE_DIRECTION_NEG_X_NEG_Y;
    }
    else if (!player_direction_x && player_direction_y)
    {
        player->move_click.move_direction = MOVE_DIRECTION_NEG_X_Y;
    }

    player->move_click.player_path_length = sqrt(pow(player->move_click.distance.x, 2) + pow(player->move_click.distance.y, 2));

    player->move_click.following_mouse_click = true;
}

bool _click_travel_finished(playerStruct *player);

/**
 * Updates player position when moving to the clicked pixel
 */
void _handle_player_mouse_moving(renderDataStruct *render_data)
{
    playerStruct *player = &render_data->player;

    if (player->move_click.player_path_length == 0.0f)
    {
        return;
    }

    float speed_ms = player->speed / 1000.0f;

    float pixels_traveled = render_data->deltaTime * speed_ms;
    float part_of_path_traveled = pixels_traveled / player->move_click.player_path_length;

    if (_click_travel_finished(player))
    {
        switch (player->move_click.move_direction)
        {
        case MOVE_DIRECTION_X_Y:
            player->x += part_of_path_traveled * player->move_click.distance.x;
            player->y += part_of_path_traveled * player->move_click.distance.y;
            break;

        case MOVE_DIRECTION_X_NEG_Y:
            player->x += part_of_path_traveled * player->move_click.distance.x;
            player->y -= part_of_path_traveled * player->move_click.distance.y;
            break;

        case MOVE_DIRECTION_NEG_X_Y:
            player->x -= part_of_path_traveled * player->move_click.distance.x;
            player->y += part_of_path_traveled * player->move_click.distance.y;
            break;

        case MOVE_DIRECTION_NEG_X_NEG_Y:
            player->x -= part_of_path_traveled * player->move_click.distance.x;
            player->y -= part_of_path_traveled * player->move_click.distance.y;
            break;
        }
    }
    else
    {
        player->move_click.following_mouse_click = false;
    }
}

walkingDirection _get_walking_direction(keyPressState key_press[KEYS_TOTAL]);
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
/**
 * Returns the direction the player is walking based on the arrow keys pressed
 */
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