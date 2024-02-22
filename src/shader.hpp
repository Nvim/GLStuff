#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>

class Shader {
public:
  unsigned int ID;

  Shader(const char *vertexPath, const char *FragmentPath);

  void use();
  // Uniform setters:
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 value) const;

private:
  void checkCompileErrors(unsigned int shader, std::string type);
};
