#version 330 core

uniform float iGlobalTime;

out vec4 FragColor;
in vec2 TexCoord;
in vec3 ourColor;

uniform sampler2D container;
uniform sampler2D huh;

void main()
{
    vec3 color = ourColor;
    // Ajouter une variation cyclique en fonction du temps
    float time = sin(2 * iGlobalTime);

    // Appliquer la variation à la composante rouge
    color.r += 0.5 * time;
    color.g += 2 * time;
    color.b += 0.1 * time;

    // Assigner la couleur finale au fragment
    FragColor = mix(texture(container, TexCoord) * vec4(color, 1.0f), texture(huh, TexCoord), 0.7);
}
