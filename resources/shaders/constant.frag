#version 400

uniform vec3 objectColor;

out vec4 out_color;

void main () {
    out_color = vec4(objectColor, 1.0);
}
