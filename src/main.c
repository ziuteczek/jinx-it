#include <SDL3/SDL.h>
#include <stdbool.h>

#include "engine.h"
#include "render.h"
#include "update.h"

int main(int argc, char **argv)
{
  SDL_Window *window;
  SDL_Renderer *renderer;

  bool launchSucces = Launch(&window, &renderer);

  if (!launchSucces)
  {
    Quit(window, renderer);
    return 1;
  }

  const float windowAspectRatio = 16.0F / 9.0F;
  SDL_SetWindowAspectRatio(window, windowAspectRatio, windowAspectRatio);

  renderDataStruct renderData = getDefaultRenderData(window, renderer);

  bool keyPress[KEYS_TOTAL] = {};

  while (!renderData.exit)
  {
    getInput(keyPress,&renderData);

    update(keyPress, &renderData);

    render(&renderData);
  }

  Quit(window, renderer);
  return 0;
}
