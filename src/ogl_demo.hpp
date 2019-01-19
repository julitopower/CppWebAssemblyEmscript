#ifndef OGL_DEMO_HPP
#define OGL_DEMO_HPP

#include <initializer_list>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

/*!
 * \brief A triangle class for OpenGL
 */
class EqTriangle {
 public:
  EqTriangle(std::initializer_list<GLfloat> il, GLfloat size);
  GLfloat vertices_[9];
};

/*!
 * \brief A context for demoing OpenGL
 *
 * At this stage its responsabilities are not fully specified.
 * I want to get to a point where I can draw something meaningful
 * and then I'll put together a proper extensible design
 */
class OpenGLContext {
 public:
  OpenGLContext(std::size_t width, std::size_t height);
  ~OpenGLContext();
  SDL_Window* window() const;
  GLuint vbo() const;
  GLuint program() const;
  GLint position_attribute() const;
  void swap_buffers();
  GLuint load_shader(const std::string& filepath, GLenum type);

 private:
  std::size_t width_;      // Window width
  std::size_t height_;     // Window height
  SDL_Window* win_;        // Windo handler
  SDL_GLContext context_;  // SDL OpenGL handler
  GLuint vertex_shader;    // A vertex shader handler
  GLuint fragment_shader;  // A fragment shader handler
  GLuint vbo_;
  GLuint program_;
  GLint position_attribute_;
  
};

/*!
 * \brief Event handling and drawing loog
 */
void ogl_loop_handler(void* ctx_ptr);

#endif // OGL_DEMO_HPP
