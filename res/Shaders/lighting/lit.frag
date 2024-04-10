#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse; // diffuse map
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct Light {
    vec4 position; // 1 = directionnal, 0 = point

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

// shader for point and directional lights depending on the position's W
void main()
{
    // ambient:
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse: diff = multiplication factor, mat.diffuse = color
    vec3 norm = normalize(Normal);
    vec3 lightDir;
    float attenuation = 1.0;
    if (light.position.w == 0.0) {
        lightDir = normalize(light.position.xyz - FragPos);
        float distance = length(light.position.xyz - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
    }
    else {
        lightDir = normalize(light.position.xyz);
    }
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    vec3 color = ambient + diffuse + specular;
    // emissive only for wood:
    // if (texture(material.specular, TexCoords).r < 0.01) {
    //     vec3 emissive = vec3(texture(material.emissive, TexCoords));
    //     color += emissive;
    // }
    FragColor = vec4(color, 1.0);
}