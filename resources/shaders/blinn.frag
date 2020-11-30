#version 400

#define MAX_LIGHTS 100

struct Light {
    vec3 position;
    vec3 lightColor;
    int lightType;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 cameraPosition;
uniform vec3 ambientColor;
uniform vec3 objectColor;

uniform sampler2D textureUnitID;

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
in vec2 uv;

out vec4 out_color;

vec3 point_light(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor) {

    const float specularStrength = 0.4;

    float dist = length(lightPosition - worldPos);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    vec3 viewDir = normalize(cameraPosition - worldPos);
    vec3 lightDir = normalize(lightPosition - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * color * attenuation;

    vec3 halfDir = normalize(lightDir + viewDir);

    float specValue = pow(max(dot(halfDir, normalVector), 0.0), 16.0);

    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return diffuse + spec;
}

vec3 directional_light(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor) {

    const float specularStrength = 0.4;

    vec3 lightDir = normalize(-lightDirection);
    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * lightColor;

    vec3 viewDir = normalize(cameraPosition - worldPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    float specValue = pow(max(dot(halfDir, normalVector), 0.0), 16.0);

    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color;
}

void main () {

    vec3 fragColor = vec3(0.0, 0.0, 0.0);

    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    vec3 worldPos = vec3(ex_worldPosition);
    vec3 normalVector = normalize(ex_worldNormal);

    vec4 tex = texture(textureUnitID, uv);
    vec3 color = vec3(tex.x, tex.y, tex.z);

    for (int index = 0; index < lightCount; ++index) {
        vec3 lightPosition = lights[index].position;
        vec3 lightColor = lights[index].lightColor;

        if (lights[index].lightType == 1) {
            fragColor += point_light(color, worldPos, normalVector, lightPosition, lightColor);
        } else if (lights[index].lightType == 2) {
            fragColor += directional_light(color, worldPos, normalVector, lightPosition, lightColor);
        }

    }

    out_color = vec4(ambientColor + fragColor, 1.0);
}
