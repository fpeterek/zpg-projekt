#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

out vec4 ex_worldPosition;
out vec4 ex_worldNormal;

layout(location=0) in vec3 vp;
layout(location=1) in vec3 normal;

void main () {
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0);
    ex_worldPosition = modelMatrix * vec4(vp, 1.0f);
    ex_worldNormal = modelMatrix * vec4(normal, 1.f); // in_Normal;
}
