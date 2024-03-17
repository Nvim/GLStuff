#pragma once

#include "glm/ext/matrix_float4x4.hpp"

typedef struct s_Matrices {
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 model;
} s_Matrices;
