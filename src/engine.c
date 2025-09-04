#include <SDL3/SDL.h>
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
    debug("SDL launch succes");

    return true;
}
void _handle_key_click(bool isKeyDown, SDL_Keycode key, keyPressState keyPress[KEYS_TOTAL]);

void get_input(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            bool isKeyDown = SDL_EVENT_KEY_DOWN == event.type;
            _handle_key_click(isKeyDown, event.key.key, keyPress);
            debug("Key pressed: %s", SDL_GetKeyName(event.key.key));
            break;
        }
        case SDL_EVENT_QUIT:
            renderData->exit = true;
            debug("Program termination requested");
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_GetWindowSize(renderData->window, &renderData->width, &renderData->height);
            debug("Window resized to x: %d, y: %d", renderData->width, renderData->height);
            break;
        }
    }
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

void quit(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    debug("SDL quited");
}