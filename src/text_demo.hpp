#ifndef TEXT_DEMO_CONTEXT_HPP
#define TEXT_DEMO_CONTEXT_HPP

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct TextContext {
  TextContext();
  ~TextContext() {
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

void loop_handler(void* ctx_ptr);

#endif // TEXT_DEMO_CONTEXT_HPP
