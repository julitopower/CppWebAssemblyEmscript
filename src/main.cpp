#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <emscripten.h>

struct Context {
  Context() {
    // Initialize SDL and TTF libraries
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create Window and Renderer
    win = SDL_CreateWindow("WebAssembly", 0, 0, 800, 600, 0);
    renderer = SDL_CreateRenderer(win, 0, SDL_RENDERER_ACCELERATED);
    
    // Load a font, define text and generate texture with text. This texture
    // is what SDL actually draws
    font = TTF_OpenFont("assets/DejaVuSansMono.ttf", 12);
    white = {255, 255, 255};
    surf = TTF_RenderText_Solid(font, sentences.back().c_str(), white);
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

  SDL_Color white;
  SDL_Window* win;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Surface* surf;
  SDL_Texture* tex;
  std::vector<std::string> sentences{"_"};
};

void loop_handler(void* ctx_ptr) {
  Context* ctx{static_cast<Context*>(ctx_ptr)};

  SDL_Event event;
  if (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_KEYDOWN) {
      auto& sentence = ctx->sentences.back();
      // Prepare the text: Remove '_' suffix, add new character, ....
      sentence.pop_back();
      int key = event.key.keysym.sym;

      switch(key) {
        case SDLK_BACKSPACE:
          if (ctx->sentences.back().size() > 0) {
            ctx->sentences.back().pop_back();
          } else if (ctx->sentences.size() > 1) {
            ctx->sentences.pop_back();
          }
          break;
        case SDLK_RETURN: ctx->sentences.push_back(""); break;
        default: ctx->sentences.back() += event.key.keysym.sym;
      }
      ctx->sentences.back() += "_";      
    }
  }


  // We will be creating a new texture, so destroy the current
  auto y = 12;
  for(const auto& sentence : ctx->sentences) {
    SDL_DestroyTexture(ctx->tex);
    SDL_FreeSurface(ctx->surf);
    // Create the new surface and texture
    ctx->surf = TTF_RenderText_Solid(ctx->font, sentence.c_str(), ctx->white);
    ctx->tex = SDL_CreateTextureFromSurface(ctx->renderer, ctx->surf); 
    // Determine the size of the text texture, so that we can
    // draw it in the right scale
    int w, h;
    SDL_QueryTexture(ctx->tex, NULL, NULL, &w, &h);
    SDL_Rect dst{0, y, w, h};
        
    // Copy the texture to the rendere and draw it
    SDL_RenderCopy(ctx->renderer, ctx->tex, nullptr, &dst);
    y += 16;
    SDL_RenderPresent(ctx->renderer);
  }
}

int main(int argc, char** argv) {
  // The Context constructor takes care of initializing SDL
  Context ctx;

  // Start the main loop. Let the browser dictate the frequency
  emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);
  return 0;
}
