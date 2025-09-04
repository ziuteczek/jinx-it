#include <SDL3/SDL.h>
#include <stdbool.h>

#include "engine.h"
#include "render.h"
#include "update.h"

int main(int argc, char **argv)
{
  SDL_Window *window;
  SDL_Renderer *renderer;

  bool launchSucces = launch(&window, &renderer);

  if (!launchSucces)
  {
    quit(window, renderer);
    return 1;
  }

  const float window_aspect_ratio = 16.0F / 9.0F;
  SDL_SetWindowAspectRatio(window, window_aspect_ratio, window_aspect_ratio);

  renderDataStruct renderData = get_default_render_data(window, renderer);

  keyPressState keyPress[KEYS_TOTAL] = {};

  while (!renderData.exit)
  {
    get_input(keyPress, &renderData);

    update(keyPress, &renderData);

    render(&renderData);
  }

  quit(window, renderer);
  return 0;
}
