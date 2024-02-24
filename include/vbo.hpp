#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VBO {
public:
  unsigned int ID;
  VBO(float *vertices, size_t size);
  void Bind();
  void Unbind();
  void Delete();
};
