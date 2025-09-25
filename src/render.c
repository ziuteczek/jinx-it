#include <SDL3/SDL.h>
#include <stdbool.h>

#include "debug.h"
#include "render.h"
#include "engine.h"
#include "config.h"

void render(renderDataStruct *renderData)
{
    SDL_Renderer *renderer = renderData->renderer;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Drawing map
    SDL_FRect map_render_pos = {
        0,
        0,
        (float)renderData->textures[TEXTURE_MAP].w * renderData->screenSizeRatio,
        (float)renderData->textures[TEXTURE_MAP].h * renderData->screenSizeRatio,
    };
    SDL_RenderTexture(renderer, renderData->textures[TEXTURE_MAP].data, NULL, &map_render_pos);

    // Drawing player
    SDL_FRect player_render_pos = {renderData->player.x * renderData->screenSizeRatio, renderData->player.y * renderData->screenSizeRatio, renderData->screenSizeRatio * (float)renderData->textures[TEXTURE_PLAYER].w, renderData->screenSizeRatio * (float)renderData->textures[TEXTURE_PLAYER].h};

    SDL_RenderTexture(renderer, renderData->textures[TEXTURE_PLAYER].data, NULL, &player_render_pos);

    SDL_RenderPresent(renderer);
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

    render_data->textures[TEXTURE_MAP] = get_texture_from_path(render_data->renderer, "../assets/map.png");
    render_data->textures[TEXTURE_PLAYER] = get_texture_from_path(render_data->renderer, "../assets/player.png");

    for (int i = 0; i < TEXTURES_TOTAL; i++)
    {
        if (!render_data->textures[i].success)
        {
            debug("Unsuccesfull while creating texture %d", i);
            return false;
        }
    }

    render_data->screenSizeRatio = ((float)render_data->height / (float)BASIC_HEIGHT + (float)render_data->width / (float)BASIC_WIDTH) / 2.0f;

    return true;
}
void free_textures(renderDataStruct *render_data)
{
    for (int i = 0; i < TEXTURES_TOTAL; i++)
    {
        SDL_DestroyTexture(render_data->textures[i].data);
    }
}