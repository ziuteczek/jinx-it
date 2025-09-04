#ifndef ENGINE_H
#define ENGINE_H

#include "config.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include "debug.h"
#include "render.h"

typedef enum keyType
{
    KEY_ARROW_UP,
    KEY_ARROW_RIGHT,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEYS_TOTAL
} keyType;

/*
 * KEY_RIGHTOWN - Key is pressed
 * KEY_UP - Key is not pressed
 */
typedef enum keyPressState
{
    KEY_STATE_DOWN,
    KEY_STATE_UP
} keyPressState;

typedef struct eventNode
{
    SDL_Event data;
    struct eventNode *next;
} eventNode;

bool launch(SDL_Window **window, SDL_Renderer **renderer);
void get_input(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData);
void quit(SDL_Window *window, SDL_Renderer *renderer);

#endif
