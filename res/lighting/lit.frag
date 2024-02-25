#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

uniform sampler2D container;

void main()
{
    // ambient:
    vec3 ambient = light.ambient * material.ambient;

    // diffuse: diff = multiplication factor, mat.diffuse = color
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // specular:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}

// void main()
// {
//     // ambient lighting:
//     float ambientStrength = 0.1;
//     vec3 ambient = ambientStrength * lightColor;
//
//     // diffuse lighting:
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(lightPos - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;
//
//     // specular lighting:
//     float specularStrength = 0.9;
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128.0);
//     vec3 specular = specularStrength * spec * lightColor;
//
//     //vec4 col = vec4(lightColor * objectColor, 1.0);
//     vec3 col = (ambient + diffuse + specular) * objectColor;
//     // FragColor = vec4(col, 1.0);
//     FragColor = mix(vec4(col, 1.0), texture(container, TexCoords), 0.2);
// }
