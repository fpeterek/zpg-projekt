#version 330

uniform mat4 modelMatrix;
layout(location=0) in vec3 vp;

void main () {
    gl_Position = modelMatrix * vec4(vp, 1.0);
}
