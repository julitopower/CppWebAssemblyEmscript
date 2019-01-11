CC=emcc
CFLAGS=-s USE_SDL=2 -s USE_SDL_TTF=2 -s WASM=1 -g3 -std=c++14
LDFLAGS=--emrun
ASSETS_DIR=assets

all: test.html

test.html: src/main.cpp src/text_demo.cpp
	emcc src/main.cpp src/text_demo.cpp --preload-file assets ${CFLAGS} -o test.html

run: test.html
	emrun --no_browser --hostname 0.0.0.0 --port 80 test.html

.PHONY:clean	
clean:
	rm -rf test*
