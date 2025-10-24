#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "debug.h"
#include "engine.h"
#include "render.h"
#include "update.h"
#include "config.h"

int main(int argc, char **argv)
{
  SDL_Window *window;
  SDL_Renderer *renderer;

  bool launch_success = launch(&window, &renderer);

  if (!launch_success)
  {
    quit(window, renderer);
    return 1;
  }

  const float window_aspect_ratio = 16.0F / 9.0F;
  bool set_window_aspect_ratio_success = SDL_SetWindowAspectRatio(window, window_aspect_ratio, window_aspect_ratio);

  if (!set_window_aspect_ratio_success)
  {
    quit(window, renderer);
    return 2;
  }

  renderDataStruct render_data;
  bool render_data_defaulting_success = set_default_render_data(&render_data, window, renderer);

  if (!set_default_render_data)
  {
    return 3;
  }

  if (!render_data_defaulting_success)
  {
    quit(window, renderer);
    debug("Setting render_data default values didn't succeed");
    return 3;
  }

  inputDataStruct input_data;

  new_missile(&render_data.missiles, &render_data.player);

  while (!input_data.exit)
  {
    get_input(&input_data);
    update(&input_data, &render_data);
    render(&render_data);
  }

  free_textures(&render_data);
  destroy_all_missiles(&render_data.missiles.data);
  free(render_data.missiles.data);

  quit(window, renderer);
  return 0;
}
