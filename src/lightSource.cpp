#include <lightSource.hpp>

void lightSource::setLightSettings(s_lightSettings &light) {
  lightSettings = light;
}

s_lightSettings lightSource::getLightSettings() { return lightSettings; }
