#ifndef GAME_H
#define GAME_H

typedef struct moveClick
{
    bool following_mouse_click;
    SDL_FPoint distance;
    SDL_FPoint direction;

    float player_path_length;
} moveClick;

/**
 * Struct to hold player sprite info
 *
 * x - player x pos
 * y - player y pos
 *
 * speed - player speed expressed in pixels per seconds
 */
typedef struct playerStruct
{
    float x;
    float y;

    int speed;

    moveClick move_click;
} playerStruct;
#endif