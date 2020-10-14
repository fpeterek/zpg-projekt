#version 400

in vec4 ex_worldPosition;
in vec4 ex_worldNormal;

out vec4 out_color;

void main () {
    vec4 lightVector = normalize(vec4(10.0, 10.0, 10.0, 10.0));
    float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);
    vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    out_color = ambient + diffuse;
    // out_color = vec4 (0.92, 0.27, 0.19, 1.0);
}
