#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "update.h"
#include "engine.h"
#include "render.h"
#include "texture.h"
#include "missile.h"

#define CAP_FRAME_RATE false

#define SQRT_2 1.41421356237309504880
#define M_PI 3.14159265358979323846

bool _any_key_pressed(keyPressState key_press[KEYS_TOTAL]);

void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *render_data);
void _handle_mouse_movement(inputDataStruct *input_data, renderDataStruct *render_data);

void _frame_timing(renderDataStruct *render_data);
void _cap_frame_rate(renderDataStruct *render_data, int fps);

void _handle_scaling(inputDataStruct *input_data, renderDataStruct *render_data);
void update_missiles(renderDataStruct *render_data);

void update(inputDataStruct *input_data, renderDataStruct *render_data)
{
    playerStruct *player = &render_data->player;

    _handle_scaling(input_data, render_data);

#if CAP_FRAME_RATE
    _cap_frame_rate(render_data, 240);
#endif

    _frame_timing(render_data);

    _handle_mouse_movement(input_data, render_data);

    if (_any_key_pressed(input_data->key_press) && render_data->deltaTime > 0)
    {
        _handle_key_press(input_data->key_press, render_data);
    }
    update_missiles(render_data);
}

void _handle_scaling(inputDataStruct *input_data, renderDataStruct *render_data)
{
    if (input_data->resize)
    {
        SDL_GetWindowSize(render_data->window, &render_data->width, &render_data->height);
        render_data->screenSizeRatio = get_screen_size_ratio(render_data->width);
    }

    input_data->mouse_pos.x /= render_data->screenSizeRatio;
    input_data->mouse_pos.y /= render_data->screenSizeRatio;
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

    // If player is currently marked as moving, updates his position
    if (player->move_click.following_mouse_click)
    {
        _handle_player_mouse_moving(render_data);
    }

    // Checks if right mouse button was pressed, if so starts player movement to clicked pixel
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

    // Distance between player and designated pixel
    player->move_click.distance.x = abs(player->move_click.direction.x - player->x);
    player->move_click.distance.y = abs(player->move_click.direction.y - player->y);

    // Is the player going forward (increasing coordinates)
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
    render_data->player.move_click.following_mouse_click = false;

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

/**
 * Missile
 */

textures _missile_type_to_texture(missilesTypes missile_type);
missileNode *getLastMissile(missileNode *missiles);
void _set_missile_direction(missileStruct *missile, playerStruct *player);

/**
 * Creates a new missile and adds it to the linked list of missiles.
 *
 * @param missiles Pointer to the missilesStruct containing the linked list and count of missiles.
 */
void new_missile(missilesStruct *missiles, playerStruct *player)
{
    missileStruct *new_missile;
    if (missiles->count == 0)
    {
        missiles->data = malloc(sizeof(missileNode));
        missiles->data->next = NULL;
        new_missile = &missiles->data->data;
    }
    else
    {
        missileNode *last_missile;
        last_missile = getLastMissile(missiles->data);
        last_missile->next = malloc(sizeof(missileNode));
        missiles->data->next = NULL;

        new_missile = &last_missile->next->data;
    }

    missilesTypes missile_type = (missilesTypes)SDL_rand(MISSILES_TOTAL);
    new_missile->texture = _missile_type_to_texture(missile_type);

    new_missile->speed = 60;

    _set_missile_direction(new_missile, player);

    missiles->count++;
}

void _set_missile_direction(missileStruct *missile, playerStruct *player)
{
    // Generating starting point
    int startingPosSeed = SDL_rand(BASIC_HEIGHT * 2 + BASIC_HEIGHT * 2);

    if (startingPosSeed < BASIC_WIDTH)
    {
        // Missile starting from bottom
        missile->pos.y = BASIC_HEIGHT;
        missile->pos.x = startingPosSeed;
    }
    else if (startingPosSeed < BASIC_WIDTH * 2)
    {
        // Missile starting from top
        missile->pos.y = 0;
        missile->pos.x = startingPosSeed - BASIC_WIDTH;
    }
    else if (startingPosSeed < BASIC_WIDTH * 2 + BASIC_HEIGHT)
    {
        // Missile starting from left
        missile->pos.y = startingPosSeed - BASIC_WIDTH * 2;
        missile->pos.x = 0;
    }
    else
    {
        // Missile starting from right
        missile->pos.y = startingPosSeed - BASIC_WIDTH * 2 - BASIC_HEIGHT;
        missile->pos.x = BASIC_WIDTH;
    }

    float distance_x = player->x - missile->pos.x;
    float distance_y = player->y - missile->pos.y;

    float distance_to_player = sqrtf(powf(distance_x, 2) + powf(distance_y, 2));

    const float missile_speed_ms = missile->speed * 0.001f;

    float part_of_distance_traveled = missile_speed_ms / distance_to_player;

    missile->pixels_per_ms_x = distance_x * part_of_distance_traveled;
    missile->pixels_per_ms_y = distance_y * part_of_distance_traveled;

    missile->angle = SDL_atan2(missile->pixels_per_ms_y, missile->pixels_per_ms_x) * (180 / SDL_PI_F) + 90.0f;
}

/**
 * This function iterates through the linked list of missiles, freeing each node's memory.
 * After all nodes are freed, the head pointer is set to NULL to indicate the list is empty.
 *
 * @param missile_head Pointer to the head pointer of the missile linked list.
 */
void destroy_all_missiles(missileNode **missile_head)
{
    missileNode *current = *missile_head;
    while (current != NULL)
    {
        missileNode *next = current->next;
        free(current);
        current = next;
    }
    *missile_head = NULL;
}
/**
 * Destroys a missile node from the linked list.
 *
 * @param prev_node Pointer to the previous node in the linked list. If the node to be destroyed is the head, this should be NULL.
 * @param to_destroy_node Pointer to the node that needs to be destroyed.
 */
void destroy_missile(missileNode *prev_node, missileNode *to_destroy_node)
{
    if (prev_node != NULL)
    {
        prev_node->next = to_destroy_node->next;
    }
    free(to_destroy_node);
}

bool is_out_of_screen(SDL_FRect obj)
{
    float max_padding = sqrtf(powf(obj.h, 2.0f) + powf(obj.w, 2.0f));
    return obj.y + max_padding < 0 || obj.y - max_padding > BASIC_HEIGHT || obj.x + max_padding < 0 || obj.x - max_padding > BASIC_WIDTH;
}

/**
 * Updates all data of the missiles
 *
 * @param render_data Pointer to the renderDataStruct
 */
void update_missiles(renderDataStruct *render_data)
{
    missileNode *current_missile_node = render_data->missiles.data;
    missileNode *prev_missile_node = NULL;

    int destroyed_missiles = 0;

    while (current_missile_node != NULL)
    {
        missileStruct *current_missile = &current_missile_node->data;
        const float speed_ms = current_missile->speed / 1000.0f;

        current_missile->pos.x += current_missile->pixels_per_ms_x * render_data->deltaTime;
        current_missile->pos.y += current_missile->pixels_per_ms_y * render_data->deltaTime;

        gameTexture missile_texture = render_data->textures[current_missile->texture];

        SDL_FRect missile_obj = {current_missile->pos.x, current_missile->pos.y, missile_texture.w, missile_texture.h};
        bool missile_out_of_range = is_out_of_screen(missile_obj);

        // Destroying missile
        if (missile_out_of_range)
        {
            missileNode *to_destroy = current_missile_node;
            current_missile_node = current_missile_node->next;

            destroy_missile(prev_missile_node, to_destroy);
            render_data->missiles.count--;
        }

        if (missile_out_of_range && prev_missile_node == NULL)
        {
            render_data->missiles.data = current_missile_node;
        }
        else if (!missile_out_of_range)
        {
            prev_missile_node = current_missile_node;
            current_missile_node = current_missile_node->next;
        }
    }

    if (render_data->missiles.count == 0)
    {
        printf("%d \n", render_data->missiles.count);
        new_missile(&render_data->missiles, &render_data->player);
    }
}
/**
 * Returns the last node in the linked list of missiles.
 *
 * @return Pointer to the last missile node, or NULL if the list is empty.
 */
missileNode *getLastMissile(missileNode *missiles)
{
    if (missiles == NULL)
    {
        return NULL;
    }

    missileNode *current = missiles;
    while (current->next != NULL)
    {
        current = current->next;
    }
    return current;
}

/**
 * Translates a missile type to its corresponding texture.
 *
 * @return The texture associated with the given missile.
 */
textures _missile_type_to_texture(missilesTypes missile)
{
    switch (missile)
    {
    case MISSILE_ROCKET:
        return TEXTURE_ROCKET;
    }
}