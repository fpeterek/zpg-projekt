cmake_minimum_required(VERSION 3.17)
project(zpg_projekt)

set(CMAKE_CXX_STANDARD 17)

link_libraries(-lglfw -lglut -lGL -ldl -pthread -lGLEW)

include_directories(/usr/include /usr/local/include include)

add_executable(
        zpg_projekt
        src/main.cpp
        src/application.cpp
        src/shader.cpp
        src/object.cpp
        src/camera.cpp
        src/mouse.cpp
        src/light.cpp
        src/model.cpp
        src/scene.cpp
        src/texture.cpp
        src/observer.cpp)

file(COPY resources DESTINATION .)
