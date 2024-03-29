
#pragma once

#include "camera.hpp"

typedef struct s_MouseInput {
  float lastMouseX = SCR_WIDTH / 2.0f;
  float lastMouseY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;
} s_MouseInput;
