# CppWebAssemblyEmscripten

WebAssembly + C++ + Emscript. The current example contains a Canvas that captures keyboard input. This is an early effort, and it has limitations:

* Doesn't handle control characters other than backspace, and return
* Wrap around is not implemented

This project will be morphing into a graphical application, and different commits are likely to reflect progress in my understanding of WebAssembly/Emscripten features

# Setup

Follow the steps in https://cplusplusbaremetal.blogspot.com/2019/01/introduction-to-webassembly-c.html to setup a fully functioning c++/Emscripten development environment.

# Build

For the moment this projects uses Makefiles to drive the build process. To build the code simply execute:

```bash
make
```

# Running

```bash
make run
```
