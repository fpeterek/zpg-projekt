#version 400

#define MAX_LIGHTS 100

struct Light {
    vec3 position;
    vec3 lightColor;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 ambientColor;
uniform vec3 objectColor;

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_color;

void main () {

    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    vec3 worldPos = vec3(ex_worldPosition);

    for (int index = 0; index < lightCount; ++index) {

        vec3 lightPosition = lights[index].position;
        vec3 lightColor = lights[index].lightColor;

        vec3 lightVector = normalize(lightPosition - vec3(ex_worldPosition));
        vec3 normalVector = normalize(ex_worldNormal);

        float dot_product = max(dot(lightVector, normalVector), 0.0);
        diffuse = diffuse + dot_product * objectColor;
        // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);
    }

    vec3 ambient = ambientColor;

    out_color = vec4(ambient + diffuse, 1.0);
    // out_color = vec4(0.92, 0.27, 0.19, 1.0);
}
