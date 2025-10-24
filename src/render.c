#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "render.h"
#include "engine.h"

void _draw_player(renderDataStruct *render_data);
void _draw_map(renderDataStruct *render_data);
void _draw_missile(renderDataStruct *render_data);

void render(renderDataStruct *render_data)
{
    SDL_Renderer *renderer = render_data->renderer;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Drawing map
    _draw_map(render_data);

    // Drawing player
    _draw_player(render_data);

    _draw_missile(render_data);

    SDL_RenderPresent(renderer);
}

void _draw_missile(renderDataStruct *render_data)
{
    missileNode *current = render_data->missiles.data;
    while (current != NULL)
    {
        missileStruct *missile = &current->data;
        gameTexture *missile_texture = &render_data->textures[missile->texture];

        SDL_FRect render_target;
        render_target.h = missile_texture->h * render_data->screenSizeRatio;
        render_target.w = missile_texture->w * render_data->screenSizeRatio;
        render_target.x = missile->pos.x * render_data->screenSizeRatio;
        render_target.y = missile->pos.y * render_data->screenSizeRatio;

        SDL_FPoint missileCenter;
        missileCenter.x = render_data->textures[missile->texture].w / 2.0;
        missileCenter.y = render_data->textures[missile->texture].h / 2.0;

        SDL_RenderTextureRotated(render_data->renderer, missile_texture->data, NULL, &render_target, missile->angle, &missileCenter, SDL_FLIP_NONE);

        current = current->next;
    }
}

void _draw_map(renderDataStruct *render_data)
{
    float screen_size_ratio = render_data->screenSizeRatio;

    float map_width = render_data->textures[TEXTURE_MAP].w;
    float map_height = render_data->textures[TEXTURE_MAP].h;

    map_width *= screen_size_ratio;
    map_height *= screen_size_ratio;

    SDL_FRect map_render_pos = {0, 0, map_width, map_height};

    SDL_RenderTexture(render_data->renderer, render_data->textures[TEXTURE_MAP].data, NULL, &map_render_pos);
}

void _draw_player(renderDataStruct *render_data)
{
    const float screen_size_ratio = render_data->screenSizeRatio;

    float player_pos_x = render_data->player.x;
    float player_pos_y = render_data->player.y;

    float player_width = (float)render_data->textures[TEXTURE_PLAYER].w;
    float player_height = (float)render_data->textures[TEXTURE_PLAYER].h;

    player_pos_x *= screen_size_ratio;
    player_pos_y *= screen_size_ratio;
    player_width *= screen_size_ratio;
    player_height *= screen_size_ratio;

    SDL_FRect player_render_pos = {player_pos_x, player_pos_y, player_width, player_height};

    SDL_RenderTexture(render_data->renderer, render_data->textures[TEXTURE_PLAYER].data, NULL, &player_render_pos);
}

void refresh_render_data_struct(renderDataStruct *renderData)
{
}
bool set_default_render_data(renderDataStruct *render_data, SDL_Window *window, SDL_Renderer *renderer)
{
    render_data->renderer = renderer;
    render_data->window = window;

    render_data->player.x = 150;
    render_data->player.y = 10;
    render_data->deltaTime = 0;
    render_data->renderTime = 0;

    render_data->player.speed = 100;

    render_data->player.move_click.following_mouse_click = false;

    bool reading_window_size_success = SDL_GetWindowSize(window, &render_data->width, &render_data->height);

    if (!reading_window_size_success)
    {
        debug("Unsuccessful while reading window size");
        return false;
    }

    render_data->screenSizeRatio = get_screen_size_ratio(render_data->width);

    render_data->textures[TEXTURE_PLAYER] = get_texture_from_path(render_data->renderer, "../assets/player.png");
    render_data->textures[TEXTURE_MAP] = get_texture_from_path(render_data->renderer, "../assets/map.png");
    render_data->textures[TEXTURE_ROCKET] = get_texture_from_path(render_data->renderer, "../assets/rocket.png");

    for (int i = 0; i < TEXTURES_TOTAL; i++)
    {
        if (!render_data->textures[i].success)
        {
            debug("Unsuccessful while creating texture %d", i);
            return false;
        }
    }

    render_data->missiles.count = 0;
    render_data->missiles.data = NULL;
    render_data->missiles.last_missile_added = 0;

    return true;
}
void free_textures(renderDataStruct *render_data)
{
    for (int i = 0; i < TEXTURES_TOTAL; i++)
    {
        SDL_DestroyTexture(render_data->textures[i].data);
    }
}