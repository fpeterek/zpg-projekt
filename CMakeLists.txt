cmake_minimum_required(VERSION 3.17)
project(zpg_projekt)

set(CMAKE_CXX_STANDARD 14)

link_libraries(-lglfw -lglut -lGL -ldl -pthread)

include_directories(/usr/include /usr/local/include include)

add_executable(zpg_projekt
        src/main.cpp include/application.hpp src/application.cpp include/shaders.hpp src/shaders.cpp)
