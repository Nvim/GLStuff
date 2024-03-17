#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "model.hpp"
#include "lighting.hpp"
#include "lightSource.hpp"

class Scene {
public:
  std::vector<Model> models;
  std::vector<lightSource> lights;
  Camera camera;
  glm::vec3 bgColor;

  // input:
  float lastMouseX = SCR_WIDTH / 2.0f;
  float lastMouseY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;
  Scene(std::vector<Model> &models, std::vector<lightSource> &lights)
      : models(models), lights(lights){};
  void setBgColor(glm::vec3 bgColor) { this->bgColor = bgColor; }
};
