#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse0;
    sampler2D specular0;
    sampler2D emissive0;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
    vec3 color;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // dir lighting :
    // color = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        color += CalcPointLight(pointLights[i], norm, viewDir, FragPos);
    }

    // emissive lighting:
    // if (texture(material.specular0, TexCoords).r <= 0.01) {
    //     color += vec3(texture(material.emissive0, TexCoords));
    // }

    FragColor = vec4(color, 1.0);
}

// doesn't attenuate
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse:
    float diff = max(dot(normal, lightDir), 0.0);

    // specular:
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse0, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular0, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    // attenuation factor:
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

    // diffuse factor:
    float diff = max(dot(normal, lightDir), 0.0);

    // specular factor:
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoords)) * attenuation;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse0, TexCoords)) * attenuation;
    vec3 specular = light.specular * spec * vec3(texture(material.specular0, TexCoords)) * attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    // spotlight intensity factor:
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff), 0.0, 1.0);

    // attenuation factor:
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

    // diffuse factor:
    float diff = max(dot(normal, lightDir), 0.0);

    // specular factor:
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoords)) * attenuation * intensity;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse0, TexCoords)) * attenuation * intensity;
    vec3 specular = light.specular * spec * vec3(texture(material.specular0, TexCoords)) * attenuation * intensity;

    return (ambient + diffuse + specular);
}
