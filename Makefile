CC=emcc
CFLAGS=-s USE_SDL=2 -s USE_SDL_TTF=2 -s WASM=1 -g3 -std=c++14
LDFLAGS=--emrun
ASSETS_DIR=assets

all: test.html

test.html: copy_shaders src/main.cpp src/text_demo.cpp src/ogl_demo.cpp src/text_demo.hpp src/ogl_demo.hpp
	emcc src/main.cpp src/text_demo.cpp src/ogl_demo.cpp --preload-file assets ${CFLAGS} -o test.html

.PHONY: copy_shaders
copy_shaders:
	cp src/*.vert ./assets
	cp src/*.frag ./assets

run: test.html
	emrun --no_browser --hostname 0.0.0.0 --port 80 test.html

.PHONY:clean	
clean:
	rm -rf test* assets/*.vert assets/*.frag
