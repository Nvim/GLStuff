#pragma once

#include "model.hpp"
#include "lighting.hpp"

class lightSource : Model {
private:
  s_lightSettings lightSettings;

public:
  lightSource(const char *path, s_lightSettings &light)
      : Model(path), lightSettings(light) {}

  void setLightSettings(s_lightSettings &light);
  s_lightSettings getLightSettings();
};
