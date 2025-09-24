#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

#include "debug.h"
#include "engine.h"
#include "render.h"
#include "update.h"

int main(int argc, char **argv)
{
  SDL_Window *window;
  SDL_Renderer *renderer;

  bool launch_succes = launch(&window, &renderer);

  if (!launch_succes)
  {
    quit(window, renderer);
    return 1;
  }

  const float window_aspect_ratio = 16.0F / 9.0F;
  SDL_SetWindowAspectRatio(window, window_aspect_ratio, window_aspect_ratio);

  renderDataStruct render_data = get_default_render_data(window, renderer);

  keyPressState key_press[KEYS_TOTAL] = {};

  
  render_data.textures[TEXTURE_MAP] = get_texture_from_path(render_data.renderer, "../assets/map.png");
  
  render_data.textures[TEXTURE_PLAYER] = get_texture_from_path(render_data.renderer, "../assets/player.png");
  

  while (!render_data.exit)
  {
    get_input(key_press, &render_data);

    update(key_press, &render_data);

    render(&render_data);
  }

  SDL_DestroyTexture(render_data.textures[TEXTURE_MAP].data);

  quit(window, renderer);
  return 0;
}
