#include "sprite.h"

#define WALL_WIDTH 50

typedef struct {
    ng_sprite_t walls[100];
    int walls_counter;
    SDL_Rect srcMale;
    SDL_Rect srcFemale;
    SDL_Rect* children;
} level;

level level_create(int width, int height);