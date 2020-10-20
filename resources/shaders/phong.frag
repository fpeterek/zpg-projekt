#version 400

uniform vec3 lightPosition;

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_color;

void main () {
    vec3 lightVector = normalize(lightPosition - vec3(ex_worldPosition));
    vec3 normalVector = normalize(ex_worldNormal);

    float dot_product = max(dot(lightVector, normalVector), 0.0);
    vec4 diffuse = dot_product * vec4(0.92, 0.27, 0.19, 1.0);
    // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    out_color = ambient + diffuse;
    // out_color = vec4(0.92, 0.27, 0.19, 1.0);
}
