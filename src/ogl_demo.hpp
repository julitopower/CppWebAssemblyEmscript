#ifndef OGL_DEMO_HPP
#define OGL_DEMO_HPP

#include <SDL2/SDL.h>

class OpenGLContext {
 public:
  OpenGLContext(std::size_t width, std::size_t height);
  ~OpenGLContext();
  SDL_Window* window() const;
  void swap_buffers();

 private:
  std::size_t width_;
  std::size_t height_;
  SDL_Window* win_;
  SDL_GLContext context_;
};

void ogl_loop_handler(void* ctx_ptr);

#endif // OGL_DEMO_HPP
