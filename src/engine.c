#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "debug.h"
#include "render.h"
/*
 * Initializes SDL, window and renderer
 * @returns true on succes, false on failure
 */
bool launch(SDL_Window **window, SDL_Renderer **renderer)
{
    bool sdlInitializationSucces = SDL_Init(SDL_INIT_VIDEO);
    if (!sdlInitializationSucces)
    {
        debug("SDL initialization failed");
        debug("%s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Don't JINX it!", 500, 500, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);

    if (*window == NULL)
    {
        debug("Window initialization failed");
        debug("%s", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, SDL_RENDERER_VSYNC_DISABLED);

    if (*renderer == NULL)
    {
        debug("Renderer initialization failed");
        debug("%s", SDL_GetError());
        return false;
    }

    debug("SDL launched succesfully");

    return true;
}

void _handle_event(SDL_Event event, inputDataStruct *input_data);

/**
 * Reads latest input from user. Keyboard (arrow keys), mouse (L,M,R buttons), program quit, window resize
 */
void get_input(inputDataStruct *input_data)
{
    SDL_Event event;

    SDL_GetMouseState(&input_data->mouse_pos.x, &input_data->mouse_pos.y);

    while (SDL_PollEvent(&event))
    {
        _handle_event(event, input_data);
    }
}

void _handle_key_click(bool isKeyDown, SDL_Keycode key, keyPressState keyPress[KEYS_TOTAL]);
void _handle_mouse_click(SDL_Event *event, inputDataStruct *input_data);

void _handle_event(SDL_Event event, inputDataStruct *input_data)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    {
        bool isKeyDown = SDL_EVENT_KEY_DOWN == event.type;
        _handle_key_click(isKeyDown, event.key.key, input_data->key_press);
        debug("Key pressed: %s", SDL_GetKeyName(event.key.key));
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        _handle_mouse_click(&event, input_data);
        break;
    case SDL_EVENT_QUIT:
        input_data->exit = true;
        debug("Program termination requested");
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        input_data->resize = true;
        break;
    }
}

void _handle_mouse_click(SDL_Event *event, inputDataStruct *input_data)
{
    bool is_button_down = event->type == SDL_EVENT_MOUSE_BUTTON_DOWN;

    switch (event->button.button)
    {
    case SDL_BUTTON_LEFT:
        input_data->mouse_press[MOUSE_BUTTON_LEFT] = is_button_down ? MOUSE_STATE_DOWN : MOUSE_STATE_UP;
        break;
    case SDL_BUTTON_MIDDLE:
        input_data->mouse_press[MOUSE_BUTTON_MIDDLE] = is_button_down ? MOUSE_STATE_DOWN : MOUSE_STATE_UP;
        break;
    case SDL_BUTTON_RIGHT:
        input_data->mouse_press[MOUSE_BUTTON_RIGHT] = is_button_down ? MOUSE_STATE_DOWN : MOUSE_STATE_UP;
        break;
    default:
        break;
    }
}

/**
 * Loads a texture from a relative path and returns a gameTexture structure.
 *
 * @param renderer The current SDL renderer.
 * @param path     Relative path to the texture file (from the game executable directory).
 * @return         A gameTexture structure containing:
 *                 - w:       Width of the texture.
 *                 - h:       Height of the texture.
 *                 - success: Whether the texture was successfully loaded.
 *                 - data:    The actual texture data.
 */
gameTexture get_texture_from_path(SDL_Renderer *renderer, char *path)
{
    gameTexture texture = {};

    SDL_Surface *new_texture_surface = IMG_Load(path);

    if (new_texture_surface == NULL)
    {
        debug("%s", SDL_GetError());
        return texture;
    }

    texture.h = new_texture_surface->h;
    texture.w = new_texture_surface->w;

    SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, new_texture_surface);

    SDL_DestroySurface(new_texture_surface);

    if (new_texture == NULL)
    {
        debug("%s", SDL_GetError());
        return texture;
    }

    SDL_SetTextureScaleMode(new_texture, SDL_SCALEMODE_NEAREST);
    texture.data = new_texture;
    texture.success = true;
    return texture;
}
void _handle_key_click(bool isKeyDown, SDL_Keycode key, keyPressState keyPress[KEYS_TOTAL])
{
    switch (key)
    {
    case SDLK_UP:
        keyPress[KEY_ARROW_UP] = isKeyDown ? KEY_STATE_DOWN : KEY_STATE_UP;
        break;
    case SDLK_LEFT:
        keyPress[KEY_ARROW_LEFT] = isKeyDown ? KEY_STATE_DOWN : KEY_STATE_UP;
        break;
    case SDLK_DOWN:
        keyPress[KEY_ARROW_DOWN] = isKeyDown ? KEY_STATE_DOWN : KEY_STATE_UP;
        break;
    case SDLK_RIGHT:
        keyPress[KEY_ARROW_RIGHT] = isKeyDown ? KEY_STATE_DOWN : KEY_STATE_UP;
        break;
    }
}

float get_screen_size_ratio(int window_width)
{
    return (float)window_width / (float)BASIC_WIDTH;
}

void quit(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    debug("SDL quited");
}