#include <emscripten.h>

#include "text_demo.hpp"

int main(int argc, char** argv) {
  // The Context constructor takes care of initializing SDL
  TextContext ctx;

  // Start the main loop. Let the browser dictate the frequency
  emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);
  return 0;
}
