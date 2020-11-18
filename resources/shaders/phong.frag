#version 400

#define MAX_LIGHTS 100

struct Light {
    vec3 position;
    vec3 lightColor;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 cameraPosition;
uniform vec3 ambientColor;
uniform vec3 objectColor;

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_color;

void main () {

    const float specularStrength = 0.4;

    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    vec3 worldPos = vec3(ex_worldPosition);
    vec3 normalVector = normalize(ex_worldNormal);

    for (int index = 0; index < lightCount; ++index) {

        vec3 lightPosition = lights[index].position;
        vec3 lightColor = lights[index].lightColor;

        float dist = length(lightPosition - worldPos);
        float attenuation = clamp(3.0 / dist, 0.0, 1.0);

        vec3 viewDir = normalize(cameraPosition - worldPos);
        vec3 lightDir = normalize(lightPosition - worldPos);
        vec3 reflectionDir = reflect(-lightDir, normalVector);

        float dot_product = dot(lightDir, normalVector);
        diffuse = diffuse + max(dot_product, 0.0) * objectColor * attenuation;
        // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);

        float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
        vec3 spec = specularStrength * specValue * lightColor;
        if (dot_product < 0.0) {
            spec = vec3(0.0);
        }
        specular = specular + attenuation * spec;
    }

    out_color = vec4(ambientColor + specular + diffuse, 1.0);
}

