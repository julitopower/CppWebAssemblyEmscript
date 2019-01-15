#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ogl_demo.hpp"
#include <SDL2/SDL_opengles2.h>

EqTriangle::EqTriangle(std::initializer_list<GLfloat> il, GLfloat size) {
  auto idx = 0U;
  GLfloat center[9];
  for(auto& val : il) {
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
    width_{width}, height_{height}, triangle_{{0.0f, 0.0f, 0.0f}, 0.5} {
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

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  
  // 3. Generate the GL context
  context_ = SDL_GL_CreateContext(win_);

  // Load the data
  // Create a Vertex Buffer Object and copy the vertex data to it
  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_.vertices_),
               triangle_.vertices_, GL_STATIC_DRAW);  
  
  // 4. Load the shaders
  GLuint vertex_shader = load_shader("assets/noop.vert", GL_VERTEX_SHADER);
  std::cout << vertex_shader << std::endl;
  GLuint fragment_shader = load_shader("assets/white.frag", GL_FRAGMENT_SHADER);
  std::cout << fragment_shader << std::endl;
  // 5. Link shaders into a program

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertex_shader);
  glAttachShader(shaderProgram, fragment_shader);
  
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  
  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
  static float red = 0.;

  // Clean the buffer with a color that changes over time
  if (red > 1.0) {
    red = 0.0;
  }
  glClearColor(1., red, red, 1.);
  red += 0.005;
  glClear(GL_COLOR_BUFFER_BIT);

  // Here is where we should do some meaningful drawing
  glDrawArrays(GL_TRIANGLES, 0, 3);
  // Finally swap the buffers
  ctx->swap_buffers();
}
