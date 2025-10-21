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

bool _any_key_pressed(keyPressState key_press[KEYS_TOTAL]);

void _handle_key_press(keyPressState key_press[KEYS_TOTAL], renderDataStruct *render_data);
void _handle_mouse_movement(inputDataStruct *input_data, renderDataStruct *render_data);

void _frame_timing(renderDataStruct *render_data);
void _cap_frame_rate(renderDataStruct *render_data, int fps);

void _handle_scaling(inputDataStruct *input_data, renderDataStruct *render_data);
void update_missles(renderDataStruct *render_data);

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
    update_missles(render_data);
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
 * Missle
 */

textures _missle_type_to_texture(misslesTypes missle_type);
missleNode *getLastMissle(missleNode *missles);

/**
 * Creates a new missile and adds it to the linked list of missiles.
 * 
 * @param missles Pointer to the misslesStruct containing the linked list and count of missiles.
 */
void new_missle(misslesStruct *missles)
{
    missleStruct *new_missle;
    if (missles->count == 0)
    {
        missles->data = malloc(sizeof(missleNode));
        missles->data->next = NULL;
        new_missle = &missles->data->data;
    }
    else
    {
        missleNode *last_missle;
        last_missle = getLastMissle(missles->data);
        last_missle->next = malloc(sizeof(missleNode));
        missles->data->next = NULL;

        new_missle = &last_missle->next->data;
    }

    misslesTypes missle_type = (misslesTypes)SDL_rand(MISSLES_TOTAL);
    new_missle->texture = _missle_type_to_texture(missle_type);

    new_missle->pos.x = SDL_rand(BASIC_WIDTH);
    new_missle->pos.y = BASIC_HEIGHT;

    new_missle->destination.x = new_missle->pos.x;
    new_missle->destination.y = 0;

    new_missle->speed = 50;

    missles->count++;
}

/**
 * This function iterates through the linked list of missiles, freeing each node's memory.
 * After all nodes are freed, the head pointer is set to NULL to indicate the list is empty.
 *
 * @param missle_head Pointer to the head pointer of the missile linked list.
 */
void destroy_all_missles(missleNode **missle_head)
{
    missleNode *current = *missle_head;
    while (current != NULL)
    {
        missleNode *next = current->next;
        free(current);
        current = next;
    }
    *missle_head = NULL;
}
/**
 * Destroys a missile node from the linked list.
 *
 * @param prev_node Pointer to the previous node in the linked list. If the node to be destroyed is the head, this should be NULL.
 * @param to_destroy_node Pointer to the node that needs to be destroyed.
 */
void destroy_missle(missleNode *prev_node, missleNode *to_destroy_node)
{
    if (prev_node != NULL)
    {
        prev_node->next = to_destroy_node->next;
    }
    free(to_destroy_node);
}

/**
 * Updates all data of the missiles
 * 
 * @param render_data Pointer to the renderDataStruct
 */
void update_missles(renderDataStruct *render_data)
{
    missleNode *current_missle_node = render_data->missles.data;
    missleNode *prev_missile_node = NULL;

    while (current_missle_node != NULL)
    {
        missleStruct *current_missle = &current_missle_node->data;
        const float speed_ms = current_missle->speed / 1000.0f;

        current_missle->pos.y -= render_data->deltaTime * speed_ms;

        gameTexture missle_texture = render_data->textures[current_missle->texture];
        bool missile_out_of_range = current_missle->pos.y < 0 - missle_texture.h;

        //Destroying missle
        if (missile_out_of_range)
        {
            missleNode *to_destroy = current_missle_node;
            current_missle_node = current_missle_node->next;

            destroy_missle(prev_missile_node, to_destroy);
        }

        if (missile_out_of_range && prev_missile_node == NULL)
        {
            render_data->missles.data = current_missle_node;
        }
        else if (!missile_out_of_range)
        {
            prev_missile_node = current_missle_node;
            current_missle_node = current_missle_node->next;
        }
    }
}
/**
 * Returns the last node in the linked list of missiles.
 * 
 * @return Pointer to the last missile node, or NULL if the list is empty.
 */
missleNode *getLastMissle(missleNode *missles)
{
    if (missles == NULL)
    {
        return NULL;
    }

    missleNode *current = missles;
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
textures _missle_type_to_texture(misslesTypes misile)
{
    switch (misile)
    {
    case MISSLE_ROCKET:
        return TEXTURE_ROCKET;
    }
}