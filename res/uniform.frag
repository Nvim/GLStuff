#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 colorUniform;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * colorUniform;
}
