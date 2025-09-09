#ifndef ENGINE_H
#define ENGINE_H

#include "config.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#include "debug.h"
#include "texture.h"

struct renderDataStruct; // FORWARD DECLARATION

typedef enum keyType
{
    KEY_ARROW_UP,
    KEY_ARROW_RIGHT,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEYS_TOTAL
} keyType;

typedef enum keyPressState
{
    KEY_STATE_UP,
    KEY_STATE_DOWN
} keyPressState;

// typedef struct eventNode
// {
//     SDL_Event data;
//     struct eventNode *next;
// } eventNode;

typedef enum mousePressState
{
    MOUSE_STATE_UP,
    MOUSE_STATE_DOWN
} mousePressState;

typedef enum mouseButtons
{
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTONS_TOTAL
} mouseButtons;

typedef struct inputDataStruct
{
    SDL_FPoint mouse_pos;

    keyPressState key_press[KEYS_TOTAL];
    mousePressState mouse_press[MOUSE_BUTTONS_TOTAL];

    bool resize;
    bool exit;
} inputDataStruct;

bool launch(SDL_Window **window, SDL_Renderer **renderer);
void reset_data_struct(inputDataStruct *input_data);
void get_input(inputDataStruct *input_data);
void quit(SDL_Window *window, SDL_Renderer *renderer);
gameTexture get_texture_from_path(SDL_Renderer *renderer, char *path);

#endif
