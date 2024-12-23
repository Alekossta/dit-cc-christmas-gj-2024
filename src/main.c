#include "engine/audio.h"
#include "engine/common.h"
#include "engine/game.h"
#include "engine/interface.h"
#include "engine/sprite.h"
#include "engine/timers.h"
#include <SDL2/SDL_image.h>

#define WIDTH 1280
#define HEIGHT 800

static struct {
  ng_game_t game;
  ng_interval_t game_tick;
  SDL_Texture *player_texture;
  ng_sprite_t player;
} ctx;

static void init(void) {
  ng_game_create(&ctx.game, "Santa Roguelike", WIDTH, HEIGHT);

  ctx.player_texture = IMG_LoadTexture(ctx.game.renderer, "res/player.png");
  ng_interval_create(&ctx.game_tick, 50);
  ng_sprite_create(&ctx.player, ctx.player_texture);
  ng_sprite_set_scale(&ctx.player, 2.0f);
  ctx.player.transform.y = HEIGHT / 2;
  ctx.player.transform.x = WIDTH / 2;
  ctx.game.level_features = level_create(WIDTH, HEIGHT);
}

// A place to handle queued events.
static void handle_event(SDL_Event *event) {
  switch (event->type) {
  case SDL_KEYDOWN:
    break;

  case SDL_MOUSEMOTION:
    break;
  }
}

static void render() {
  SDL_RenderClear(ctx.game.renderer);
  ng_sprite_render(&ctx.player, ctx.game.renderer);
  SDL_SetRenderDrawColor(ctx.game.renderer, 255, 255, 255, 255);
  for (int i = 0; i < ctx.game.level_features.walls_counter; i++) {
    SDL_Rect rect = {
        (int)ctx.game.level_features.walls[i].transform.x,
        (int)ctx.game.level_features.walls[i].transform.y,
        (int)ctx.game.level_features.walls[i].transform.w,
        (int)ctx.game.level_features.walls[i].transform.h
    };
    SDL_RenderFillRect(ctx.game.renderer, &rect);
  }

  // Update the screen after all rendering
  SDL_RenderPresent(ctx.game.renderer);
}

// Will be updated when obstacles are added to game
static SDL_FRect move_sprite(ng_game_t game, ng_sprite_t *player, double dx,
                             double dy) {
  // Maximum x, y player can move to                              
  double maxx = WIDTH - player->transform.w;
  double maxy = HEIGHT - player->transform.h;
  // Minimum x, y player can move to
  double minx = 0, miny = 0;
  
  // Middle point of player
  double middlex = player->transform.x + 0.5*player->transform.w;
  double middley = player->transform.y + 0.5*player->transform.h;

  for (int i = 0; i < game.level_features.walls_counter; i++) {
    ng_sprite_t wall = game.level_features.walls[i];
    // If player is just under or above the wall
    if (wall.transform.x <= middlex && middlex <= wall.transform.x + wall.transform.w) {
        // If player is above the wall
        if (wall.transform.y >= player->transform.y + player->transform.h) {
            // Player can move up to the upper side of the wall
            maxy = fmin(maxy, wall.transform.y - player->transform.h);
        } else if (wall.transform.y + wall.transform.h <= player->transform.y) { // If player is under the wall
            // Player can move up to the lower side of the wall
            miny = fmax(miny, wall.transform.y + wall.transform.h);
        }
    }
    // If player is just on the left or on the right of the wall
    if (wall.transform.y <= middley && middley <= wall.transform.y + wall.transform.h) {
        // If player is on the left of the wall
        if (wall.transform.x >= player->transform.x + player->transform.w) {
            // The player can move up to the left side of the wall
            maxx = fmin(maxx, wall.transform.x - player->transform.w);
        } else if (wall.transform.x + wall.transform.w <= player->transform.x) { // If player is on the right of the wall
            // The player can move up to the right side of the wall
            minx = fmax(minx, wall.transform.x + wall.transform.w);
        }
    }
  }

  // Move the player
  player->transform.x =
      fmin(maxx, player->transform.x + dx);
  player->transform.x = fmax(minx, player->transform.x);

  player->transform.y =
      fmin(maxy, player->transform.y + dy);
  player->transform.y = fmax(miny, player->transform.y);

}

static void update(float delta) {
  // Handling "continuous" events, which are now repeatable
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  const int speed = 500;

  if (keys[SDL_SCANCODE_A]) {
    move_sprite(ctx.game, &ctx.player, -speed * delta, 0);
  }
  if (keys[SDL_SCANCODE_D]) {
    move_sprite(ctx.game, &ctx.player, speed * delta, 0);
  }

  if (keys[SDL_SCANCODE_W]) {
    move_sprite(ctx.game, &ctx.player, 0, -speed * delta);
  }
  if (keys[SDL_SCANCODE_S]) {
    move_sprite(ctx.game, &ctx.player, 0, speed * delta);
  }

  render();
}

int main() {
  init();
  ng_game_start_loop(&ctx.game, handle_event, update);
}