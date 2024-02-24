#pragma once

#include "vbo.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VAO {
public:
  unsigned int ID;
  VAO();
  void Bind();

  // Uses GLVertexAttribPointer on VBO. Layout = index variable in shader
  void LinkVBO(VBO &VBO, unsigned int layout, unsigned int numComponents,
               GLenum type, size_t stride, void *offset);
  void Unbind();
  void Delete();
};
