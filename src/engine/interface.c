#include "interface.h"
#include <SDL2/SDL.h>

// Default font color
static SDL_Color white = {255, 255, 255, 255};

void ng_label_create(ng_label_t *label, TTF_Font *font, unsigned int wrap_length)
{
    // Initializing to NULL so that we don't free garbage
    label->sprite.texture = NULL;

    label->font = font;
    label->wrap_length = wrap_length;
}

void ng_label_set_content(ng_label_t *label, SDL_Renderer *renderer, const char *content)
{
    // Avoid the memory leak
    SDL_DestroyTexture(label->sprite.texture);
    
    SDL_Surface *surface = label->wrap_length > 0
        ? TTF_RenderText_Solid_Wrapped(label->font, content, white, label->wrap_length)
        : TTF_RenderText_Solid(label->font, content, white);

    ng_sprite_create(&label->sprite, SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);
}

void ng_label_destroy(ng_label_t *label)
{
    SDL_DestroyTexture(label->sprite.texture);
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}
