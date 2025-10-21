#ifndef GAME_H
#define GAME_H

/**
 * Enum to hold the direction of mouse movement
 */
typedef enum moveDirection
{
    MOVE_DIRECTION_X_Y,          // +X +Y
    MOVE_DIRECTION_X_NEG_Y,     // +X -Y
    MOVE_DIRECTION_NEG_X_NEG_Y, // -X -Y
    MOVE_DIRECTION_NEG_X_Y      // -X +Y
} moveDirection;
/**
 * Struct to hold player movement info
 * 
 * following_mouse_click - is the player currently moving towards a mouse click
 * distance - distance to the mouse click
 * direction - direction to the mouse click
 * move_direction - enum to hold the direction of movement
 * player_path_length - total length of the path to the mouse click
 */
typedef struct moveClick
{
    bool following_mouse_click;
    SDL_FPoint distance;
    SDL_FPoint direction;
    
    moveDirection move_direction;

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