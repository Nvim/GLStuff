#pragma once

#include "shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

class Texture {
public:
  unsigned int ID;
  GLenum type;
  GLenum slot;
  Texture(const char *file, GLenum type, GLenum slot, GLenum format,
          GLenum pixel_type);
  void TexUnit(Shader &shader, const char *uniform, unsigned int unit);
  void Bind();
  void Unbind();
  void Delete();
};
