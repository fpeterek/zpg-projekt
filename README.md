# Build

School PC:

```
g++ main.cpp -std=c++17 -I ~/glfw/usr/local/include/ -I . -L ~/glfw/usr/local/lib/ -lglfw3 -lglut -lGL -ldl -pthread
```

My PC:

Manjaro, libraries installed using Pacman.

```
g++ -std=c++17 src/* -I include -lglfw -lglut -lGL -ldl -lSOIL -pthread -lassimp -lGLEW -O3 -o zpg
```

macOS:

```
g++ src/main.cpp -std=c++17 -pthread -lglfw -ldl -lglut -framework OpenGL -O3 -o zpg
```

