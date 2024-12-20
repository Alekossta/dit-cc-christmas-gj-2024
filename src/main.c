#include <SDL2/SDL_image.h>
#include "engine/game.h"
#include "engine/common.h"
#include "engine/sprite.h"
#include "engine/interface.h"
#include "engine/timers.h"
#include "engine/audio.h"

#define WIDTH 1280
#define HEIGHT 800

static struct
{
    ng_game_t game;
    ng_interval_t game_tick;
    SDL_Texture *player_texture;
    ng_sprite_t player;
} ctx;

static void init(void)
{
    ng_game_create(&ctx.game, "Santa Roguelike", WIDTH, HEIGHT);

    ctx.player_texture = IMG_LoadTexture(ctx.game.renderer, "res/player.png");
    ng_interval_create(&ctx.game_tick, 50);
    ng_sprite_create(&ctx.player, ctx.player_texture);
    ng_sprite_set_scale(&ctx.player, 2.0f);
    ctx.player.transform.y = HEIGHT / 2;
    ctx.player.transform.x = WIDTH / 2;
}

// A place to handle queued events.
static void handle_event(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
        break;

    case SDL_MOUSEMOTION:
        break;
    }
}

static void render()
{
    ng_sprite_render(&ctx.player, ctx.game.renderer);
}

static void update(float delta)
{
    // Handling "continuous" events, which are now repeatable
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    const int speed = 500;

    if (keys[SDL_SCANCODE_A]) ctx.player.transform.x -= speed * delta;
    if (keys[SDL_SCANCODE_D]) ctx.player.transform.x += speed * delta;

    if (keys[SDL_SCANCODE_W]) ctx.player.transform.y -= speed * delta;
    if (keys[SDL_SCANCODE_S]) ctx.player.transform.y += speed * delta;

    render();
}


int main()
{
    init();
    ng_game_start_loop(&ctx.game,
            handle_event, update);
}