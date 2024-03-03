#pragma once

#include "shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

class Texture {
public:
  unsigned int ID;  // GL object ID
  const char *type; // diffuse, specular or emissive
  GLenum slot;      // GL_TEXTURE0 to 15
  const char *path;
  Texture(const char *file, const char *textureType, GLenum slot);
  void TexUnit(Shader &shader, const char *uniform, unsigned int unit);
  void Bind();
  void Unbind();
  void Delete();
};
