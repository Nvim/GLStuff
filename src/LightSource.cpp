#include <LightSource.hpp>
#include <bits/stdc++.h>

LightSource::LightSource(s_LightSettings &lightSettings) {
  this->lightSettings = lightSettings;
}

void LightSource::setLightSettings(s_LightSettings &ls) {
  this->lightSettings = ls;
}

s_LightSettings &LightSource::getLightSettings() { return this->lightSettings; }

void LightSource::Update(const glm::mat4 pos) {
  lightSettings.position = pos[3];
}
