CC=emcc
CLAGS=-s WASM=1 -O3 -g3
LDFLAGS=--emrun

all: test.html

test.html: src/main.cpp
	emcc src/main.cpp -s WASM=1 -g3 -o test.html

run: test.html
	emrun --no_browser --hostname 0.0.0.0 --port 80 test.html

.PHONY:clean	
clean:
	rm -rf test*
