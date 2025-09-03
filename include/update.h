#ifndef UPDATE_H
#define UPDATE_H

#include <SDL3/SDL.h>

#include "engine.h"
#include "render.h"

void update(keyPressState keyPress[KEYS_TOTAL], renderDataStruct *renderData);
#endif