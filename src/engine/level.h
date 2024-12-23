#include "sprite.h"

#define WALL_WIDTH 50
#define PLATFORM_RADIUS 50

typedef struct {
    ng_sprite_t platform;
    bool red;
} sprite_platform;

typedef struct {
    ng_sprite_t walls[100];
    int walls_counter;
    sprite_platform platforms[100];
    int platforms_counter;
    SDL_Rect srcMale;
    SDL_Rect srcFemale;
    SDL_Rect* children;
} level;

level level_create(int width, int height);