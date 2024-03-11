#pragma once

#include "model.hpp"
#include "lighting.hpp"

class lightSource : Model {
public:
  s_lightSettings lightSettings;
  lightSource(const char *path, s_lightSettings &light)
      : Model(path), lightSettings(light) {}

  void Draw(Shader &shader, Camera &camera, glm::mat4 model,
            Texture *texture = nullptr) override;

  void setLightSettings(s_lightSettings &light);
};
