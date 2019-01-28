#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "ogl_demo.hpp"
#include <SDL2/SDL_opengles2.h>

EqTriangle::EqTriangle(std::initializer_list<GLfloat> center_l, GLfloat size) {
  if (center_l.size() != 3) {
    throw std::invalid_argument{"Center must have 3 coordinates"};
  }
  auto idx = 0U;
  GLfloat center[9];
  for(auto& val : center_l) {
    center[idx++] = val;
  }
  vertices_[0] = center[0];
  vertices_[1] = center[1] + size;
  vertices_[2] = center[2];

  vertices_[3] = center[0] - size;
  vertices_[4] = center[1];
  vertices_[5] = center[2];

  vertices_[6] = center[0] + size;
  vertices_[7] = center[1];
  vertices_[8] = center[2];        
}

OpenGLContext::OpenGLContext(std::size_t width, std::size_t height):
    width_{width}, height_{height} {
  ////////////////////////////////////////////////////////////////////////////////
  // Initialize SDL and create OpenGL window
  ////////////////////////////////////////////////////////////////////////////////
  SDL_Init(SDL_INIT_VIDEO);
  win_ = SDL_CreateWindow("OpenGL WebAssembly",
                          0,
                          0,
                          width_,
                          height_,
                          SDL_WINDOW_OPENGL);
  // Set OpenGL attributes before getting the context
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  // Generate the GL context
  context_ = SDL_GL_CreateContext(win_);
  ////////////////////////////// SDL initialization end ////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Shader loading and program linking
  //////////////////////////////////////////////////////////////////////////////
  GLuint vertex_shader = load_shader("assets/noop.vert", GL_VERTEX_SHADER);
  GLuint fragment_shader = load_shader("assets/white.frag", GL_FRAGMENT_SHADER);

  // Link shaders into a program
  program_ = glCreateProgram();
  glAttachShader(program_, vertex_shader);
  glAttachShader(program_, fragment_shader);
  
  glLinkProgram(program_);
  glUseProgram(program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  position_attribute_ = glGetAttribLocation(program_, "position");
  ///////////////// Shader loading end /////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Data loading and VBO initialization
  //////////////////////////////////////////////////////////////////////////////
  std::vector<EqTriangle> triangles{};
  triangles.push_back(EqTriangle{{0.0f, 0.0f, 0.0f}, 0.5});
  triangles.push_back(EqTriangle{{0.75f, 0.0f, 0.0f}, 0.20});  

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangles.size() * sizeof(EqTriangle),
               reinterpret_cast<GLfloat*>(triangles.data()), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLContext::~OpenGLContext() {
  SDL_GL_DeleteContext(context_);
  SDL_DestroyWindow(win_);
  SDL_Quit();
}

SDL_Window* OpenGLContext::window() const {
  return win_;
}

GLuint OpenGLContext::vbo() const {
  return vbo_;
}

GLuint OpenGLContext::program() const {
  return program_;
}

GLint OpenGLContext::position_attribute() const {
  return position_attribute_;
}

void OpenGLContext::swap_buffers() {
  SDL_GL_SwapWindow(win_);  
}

GLuint OpenGLContext::load_shader(const std::string& filepath, GLenum type) {
  // Read file into string
  std::ifstream is{filepath};
  assert(is.good());
  std::stringstream ss;
  ss << is.rdbuf();
  GLuint shader = glCreateShader(type);
  const std::string s = ss.str();
  auto src = s.c_str();

  // Load/compile shader
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  return shader;
}

void ogl_loop_handler(void* ctx_ptr) {
  OpenGLContext* ctx = static_cast<OpenGLContext*>(ctx_ptr);

  // Clear buffer
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw
  glUseProgram(ctx->program());
  glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo());
  glEnableVertexAttribArray(ctx->position_attribute());
  glVertexAttribPointer(ctx->position_attribute(), 3, GL_FLOAT, GL_FALSE, 0, 0);  
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(ctx->position_attribute());
  
  // Finally swap the buffers
  ctx->swap_buffers();
}
