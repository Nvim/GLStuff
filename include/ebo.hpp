#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class EBO {
public:
  unsigned int ID;
  EBO(std::vector<unsigned int> &indices);
  void Bind();
  void Unbind();
  void Delete();
};
