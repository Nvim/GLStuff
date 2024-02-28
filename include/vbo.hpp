#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec2 TexCoords;
  glm::vec3 Normal;
};

class VBO {
public:
  unsigned int ID;
  VBO(std::vector<Vertex> &vertices);
  void Bind();
  void Unbind();
  void Delete();
};
