#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EBO {
public:
  unsigned int ID;
  EBO(GLuint *indices, GLsizeiptr size);
  void Bind();
  void Unbind();
  void Delete();
};
