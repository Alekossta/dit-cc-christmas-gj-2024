#include "level.h"

level level_create(int width, int height) {
  level l;
  SDL_FRect transform = {0.1*width, 0, WALL_WIDTH, 0.8*height};
  SDL_Rect src = {0, 0};
  l.walls[0] = (ng_sprite_t){NULL, src, transform};

  transform = (SDL_FRect){0.25*width, 0.8*height - WALL_WIDTH, 0.75*width, WALL_WIDTH};
  src = (SDL_Rect){0, 0};
  l.walls[1] = (ng_sprite_t){NULL, src, transform};
  l.walls_counter = 2;
  return l;
}