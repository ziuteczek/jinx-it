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

/*
 * KEY_DOWN - Key is pressed
 * KEY_UP - Key has been released 1 frame ago.
 * KEY_OFF _ Key is not pressed
 */
typedef enum keyPressState
{
    KEY_OFF,
    KEY_DOWN,
    KEY_UP
} keyPressState;

typedef struct eventNode
{
    SDL_Event data;
    struct eventNode *next;
} eventNode;

bool Launch(SDL_Window **window, SDL_Renderer **renderer);
void getInput(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData);
void Quit(SDL_Window *window, SDL_Renderer *renderer);

#endif
