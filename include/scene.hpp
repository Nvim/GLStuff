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
  glm::vec3 bgColor;
  bool flashLight = false;
  bool dirLight = true;
  // Camera camera;
  Scene(std::vector<Model> &models, std::vector<lightSource> &lights)
      : models(models), lights(lights){};
  void setBgColor(glm::vec3 bgColor) { this->bgColor = bgColor; }
};
