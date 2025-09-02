#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "debug.h"
#include "render.h"
/*
 * @returns true on succes, false on failure
 */
bool Launch(SDL_Window **window, SDL_Renderer **renderer)
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

    *renderer = SDL_CreateRenderer(*window, NULL);

    if (*renderer == NULL)
    {
        debug("Renderer initialization failed");
        debug("%s", SDL_GetError());
        return false;
    }
    debug("SDL Launch succes");

    return true;
}
/*
 * @return Node list of events
 */
void getInput(bool keyPress[KEYS_TOTAL], renderDataStruct *renderData)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            bool keyStatus = SDL_EVENT_KEY_DOWN == event.type;
            _handleKeyClick(keyStatus, event.key.key, keyPress);
            break;
        }
        case SDL_EVENT_QUIT:
            renderData->exit = true;
            debug("Recived program termination request");
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_GetWindowSize(renderData->window, &renderData->width, &renderData->height);
            debug("Window resized to x: %d, y: %d", renderData->width, &renderData->height);
            break;
        }
    }
}

void _handleKeyClick(bool keyPressStatus, SDL_Keycode key, bool keyPress[KEYS_TOTAL])
{
    switch (key)
    {
    case SDLK_W:
        keyPress[KEY_W] = keyPressStatus;
        break;
    case SDLK_A:
        keyPress[KEY_A] = keyPressStatus;
        break;
    case SDLK_S:
        keyPress[KEY_S] = keyPressStatus;
        break;
    case SDLK_D:
        keyPress[KEY_D] = keyPressStatus;
        break;
    }
}

void Quit(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    debug("SDL Quited");
}