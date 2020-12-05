#version 400

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_color;

void main() {
    vec4 green = vec4(0.22, 0.59, 0.32, 1.0);
    vec4 brown = vec4(0.59, 0.37, 0.22, 1.0);
    if (ex_worldPosition.y < 4) {
        out_color = green;
    } else if (ex_worldPosition.y > 8) {
        out_color = brown;
    } else {
        out_color = brown;
    }
}
