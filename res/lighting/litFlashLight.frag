#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse; // diffuse map
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct Light {
    vec4 position;
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

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

// shader for flashlight/spotlight
void main()
{
    vec3 color = vec3(0.0);

    // diffuse: theta = multiplication factor, mat.diffuse = color
    vec3 lightDir = normalize(light.position.xyz - FragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float distance = length(light.position.xyz - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
    diffuse *= intensity;
    specular *= intensity;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    color = ambient + diffuse + specular;

    // if (texture(material.specular, TexCoords).r < 0.01) {
    //     vec3 emissive = vec3(texture(material.emissive, TexCoords));
    //     color += emissive;
    // }
    FragColor = vec4(color, 1.0);
}
