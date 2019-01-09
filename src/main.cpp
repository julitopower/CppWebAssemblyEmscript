#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <emscripten.h>

struct Context {
  SDL_Window* win;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Surface* surf;
  SDL_Texture* tex;

  Context() {
    // Initialize SDL and TTF libraries
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create Window and Renderer
    win = SDL_CreateWindow("WebAssembly test", 0, 0, 800, 600, 0);
    renderer = SDL_CreateRenderer(win, 0, 0);
    
    // Load a font, define text and generate texture with text. This texture
    // is what SDL actually draws
    font = TTF_OpenFont("assets/DejaVuSansMono.ttf", 50);
    SDL_Color white = {255, 255, 255};
    surf = TTF_RenderText_Solid(font, "WebAssembly rocks!", white);
    tex = SDL_CreateTextureFromSurface(renderer, surf);    
  }
  
  ~Context() {
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();    
  }
};

void loop_handler(void* ctx_ptr) {
  Context* ctx{static_cast<Context*>(ctx_ptr)};

  // Determine the size of the text texture, so that we can
  // draw it in the right scale
  int w, h;
  SDL_QueryTexture(ctx->tex, NULL, NULL, &w, &h);
  SDL_Rect dst{0, 0, w, h};

  // Copy the texture to the rendere and draw it
  SDL_RenderCopy(ctx->renderer, ctx->tex, nullptr, &dst);
  SDL_RenderPresent(ctx->renderer);  
}

int main(int argc, char** argv) {
  // The Context constructor takes care of initializing SDL
  Context ctx;

  // Start the main loop. Let the browser dictate the frequency
  emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);
  return 0;
}
