#ifndef OGL_DEMO_HPP
#define OGL_DEMO_HPP

#include <initializer_list>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>


class EqTriangle {
 public:
  EqTriangle(std::initializer_list<GLfloat> il, GLfloat size);
  GLfloat vertices_[9];
};

class OpenGLContext {
 public:
  OpenGLContext(std::size_t width, std::size_t height);
  ~OpenGLContext();
  SDL_Window* window() const;
  void swap_buffers();
  GLuint load_shader(const std::string& filepath, GLenum type);

 private:
  std::size_t width_;
  std::size_t height_;
  SDL_Window* win_;
  SDL_GLContext context_;
  EqTriangle triangle_;
  GLuint vertex_shader;
  GLuint fragment_shader;  
  
};

void ogl_loop_handler(void* ctx_ptr);

#endif // OGL_DEMO_HPP
