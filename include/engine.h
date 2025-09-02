#ifndef ENGINE_H
#define ENGINE_H

#include "config.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include "debug.h"
#include "render.h"

typedef enum keyType
{
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEYS_TOTAL
} keyType;

typedef enum keyPress
{
    KEY_DOWN,
    KEY_UP
} keyPress;

typedef struct eventNode
{
    SDL_Event data;
    struct eventNode *next;
} eventNode;

bool Launch(SDL_Window **window, SDL_Renderer **renderer);
void getInput(bool keyPress[KEYS_TOTAL], renderDataStruct *renderData);
void Quit(SDL_Window *window, SDL_Renderer *renderer);

#endif
