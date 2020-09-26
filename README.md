# Build

School PC:

```
g++ main.cpp -std=c++11 -I ~/glfw/usr/local/include/ -I . -L ~/glfw/usr/local/lib/ -lglfw3 -lglut -lGL -ldl -pthread
```

My PC:

Manjaro, libraries installed using Pacman.

```
g++ -std=c++11 src/* -I include -lglfw -lglut -lGL -ldl -pthread -lGLEW -O3 -o zpg
```

macOS:

```
g++ src/main.cpp -std=c++11 -pthread -lglfw -ldl -lglut -framework OpenGL
```

