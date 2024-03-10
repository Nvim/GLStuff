#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>

typedef struct s_lightSettings {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 position;

  float constant;
  float linear;
  float quadratic;
} s_lightSettings;
