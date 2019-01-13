#include <iostream>
#include "ogl_demo.hpp"
#include <SDL2/SDL_opengles2.h>

OpenGLContext::OpenGLContext(std::size_t width, std::size_t height):
    width_{width}, height_{height} {
  // 1. Initialize SDK and create OpenGL window
  SDL_Init(SDL_INIT_VIDEO);
  win_ = SDL_CreateWindow("OpenGL WebAssembly",
                          0,
                          0,
                          width_,
                          height_,
                          SDL_WINDOW_OPENGL);


  //2. Set OpenGL attributes before getting the context
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // 3. Generate the GL context
  context_ = SDL_GL_CreateContext(win_);

  // 4. TODO: Load the shaders
}

OpenGLContext::~OpenGLContext() {
  SDL_GL_DeleteContext(context_);
  SDL_DestroyWindow(win_);
  SDL_Quit();
}

SDL_Window* OpenGLContext::window() const {
  return win_;
}

void OpenGLContext::swap_buffers() {
  SDL_GL_SwapWindow(win_);  
}

void ogl_loop_handler(void* ctx_ptr) {
  OpenGLContext* ctx = static_cast<OpenGLContext*>(ctx_ptr);
  static float red = 0.;

  // Clean the buffer with a color that changes over time
  if (red > 1.0) {
    red = 0.0;
  }
  glClearColor(1., red, red, 1.);
  red += 0.005;
  glClear(GL_COLOR_BUFFER_BIT);

  // Here is where we should do some meaningful drawing
  
  // Finally swap the buffers
  ctx->swap_buffers();
}
