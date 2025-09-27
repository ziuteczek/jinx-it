#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "render.h"
#include "engine.h"

void _draw_player(renderDataStruct *render_data);
void _draw_map(renderDataStruct *render_data);
void _draw_missle(renderDataStruct *render_data);

void render(renderDataStruct *render_data)
{
    SDL_Renderer *renderer = render_data->renderer;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Drawing map
    _draw_map(render_data);

    // Drawing player
    _draw_player(render_data);

    _draw_missle(render_data);

    SDL_RenderPresent(renderer);
}

void _draw_missle(renderDataStruct *render_data)
{
    for (int i = 0; i < render_data->missles.count; i++)
    {
        missleStruct *missle = &render_data->missles.data[i];
        gameTexture *missle_texture = &render_data->textures[missle->texture];

        SDL_FRect render_target;

        render_target.h = missle_texture->h * render_data->screenSizeRatio;
        render_target.w = missle_texture->w * render_data->screenSizeRatio;
        render_target.x = missle->pos.x * render_data->screenSizeRatio;
        render_target.y = missle->pos.y * render_data->screenSizeRatio;

        SDL_RenderTexture(render_data->renderer, missle_texture->data, NULL, &render_target);
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

    float player_pos_x = render_data->player.x - render_data->textures[TEXTURE_PLAYER].w / 2;
    float player_pos_y = render_data->player.y - render_data->textures[TEXTURE_PLAYER].h / 2;

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

    render_data->player.x = 0;
    render_data->player.y = 0;
    render_data->deltaTime = 0;
    render_data->renderTime = 0;

    render_data->player.speed = 100;

    render_data->player.move_click.following_mouse_click = false;

    bool reading_window_size_succes = SDL_GetWindowSize(window, &render_data->width, &render_data->height);

    if (!reading_window_size_succes)
    {
        debug("Unsuccesfull while reading window size");
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
            debug("Unsuccesfull while creating texture %d", i);
            return false;
        }
    }

    render_data->missles.count = 0;
    render_data->missles.data = NULL;
    render_data->missles.last_missle_added = 0;

    return true;
}
void free_textures(renderDataStruct *render_data)
{
    for (int i = 0; i < TEXTURES_TOTAL; i++)
    {
        SDL_DestroyTexture(render_data->textures[i].data);
    }
}