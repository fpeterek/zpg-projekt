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
uniform sampler2D textureUnitID;

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
in vec2 uv;

out vec4 out_color;

void main () {

    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    vec3 worldPos = vec3(ex_worldPosition);
    vec3 normalVector = normalize(ex_worldNormal);

    for (int index = 0; index < lightCount; ++index) {

        vec3 lightPosition = lights[index].position;
        vec3 lightColor = lights[index].lightColor;

        float dist = length(lightPosition - worldPos);
        float attenuation = clamp(3.0 / dist, 0.0, 1.0);

        vec3 lightVector = normalize(lightPosition - vec3(ex_worldPosition));

        float dot_product = max(dot(lightVector, normalVector), 0.0);
        diffuse = diffuse + dot_product * objectColor * attenuation;
        // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);
    }

    out_color = texture(textureUnitID, uv); // * vec4(ambientColor + diffuse, 1.0);
}
