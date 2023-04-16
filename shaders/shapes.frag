#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec4 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight;

uniform Material material;
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPos;

uniform vec4 viewPos;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = vec3(normalize(viewPos - FragPos));

    vec3 result = calcDirLight(dirLight, normal, viewDir);
    result += calcPointLight(pointLight, normal, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    if (length(light.direction) == 0) {
        return vec3(0.0, 0.0, 0.0);
    }
    vec3 diffuseTexColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specularTexColor = vec3(texture(material.texture_specular1, TexCoords));

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * diffuseTexColor;
    vec3 diffuse = light.diffuse * diff * diffuseTexColor;
    vec3 specular = light.specular * spec * specularTexColor;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    if (light.position.w == 0.0) {
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 lightDir = vec3(normalize(light.position - FragPos));

    vec3 diffuseTexColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specularTexColor = vec3(texture(material.texture_specular1, TexCoords));

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * diffuseTexColor;
    vec3 diffuse = light.diffuse * diff * diffuseTexColor;
    vec3 specular = light.specular * spec * specularTexColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
