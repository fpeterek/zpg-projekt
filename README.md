## Build

Arch, X11, libraries installed using `pacman`. Should be the same for all Linux distributions,
just install the appropriate libraries using your package manager or compile them from sources.
I don't know whether it's going to work under Wayland.

```sh
g++ -std=c++17 src/* -I include -lglfw -lglut -lGL -ldl -lSOIL -pthread -lassimp -lGLEW -O3 -o zpg
```

## Run

```sh
./zpg
```

## Incomplete Instructions

### School PC

I never made the attempt to make the final version of the project work on the school PCs, but
I had to install some of the libraries to my home directory. This of course required building the
libraries from source. You might be able to make it work by following up on my initial efforts.

```sh
g++ main.cpp -std=c++17 -I ~/glfw/usr/local/include/ -I . -L ~/glfw/usr/local/lib/ -lglfw3 -lglut -lGL -ldl -pthread
```

### macOS

Same story as the school PC, I finished the project on Linux, but feel free to follow up on my efforts.

```sh
g++ src/main.cpp -std=c++17 -pthread -lglfw -ldl -lglut -framework OpenGL -O3 -o zpg
```

