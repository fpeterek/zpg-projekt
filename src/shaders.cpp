//
// Created by Filip Peterek on 22/09/2020.
//

#include "shaders.hpp"

namespace Shaders {

    const char * vertexShader =
            "#version 330\n"
            "layout(location=0) in vec3 vp;"
            "void main () {"
            "     gl_Position = vec4 (vp, 1.0);"
            "}";

    const char * fragmentShader =
            "#version 330\n"
            "out vec4 frag_colour;"
            "void main () {"
            "     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
            "}";
}
