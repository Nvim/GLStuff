#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
uniform float offset;

void main()
{
  float x = aPos.x;
  if(x < 0){
    x = x - offset;
  }
  else if(x > 0){
    x = x + offset;
  }

  float y = aPos.y;
  if(x < 0){
    y = y - offset;
  }
  else if(y > 0){
    y = y + offset;
  }

gl_Position = vec4(x, y, aPos.z, 1.0f);
ourColor = aColor;
}                                    
