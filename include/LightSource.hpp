#pragma once

#include "IObserver.hpp"
#include "s_LightSettings.hpp"

class LightSource : public IObserver {
private:
  s_LightSettings lightSettings;

public:
  LightSource(s_LightSettings &ls);

  void Update(const glm::mat4 pos);
  s_LightSettings &getLightSettings();
  void setLightSettings(s_LightSettings &ls);
};
