#include <LightSource.hpp>
#include <bits/stdc++.h>

LightSource::LightSource(s_LightSettings lightSettings) {
  this->lightSettings = lightSettings;
}

void LightSource::setLightSettings(s_LightSettings &ls) {
  this->lightSettings = ls;
}

s_LightSettings LightSource::getLightSettings() { return lightSettings; }

void LightSource::RegisterObserver(IObserver *observer) {
  observers.push_back(observer);
}
void LightSource::RemoveObserver(IObserver *o) {
  observers.erase(std::remove(observers.begin(), observers.end(), o),
                  observers.end());
}

void LightSource::NotifyObservers() {
  for (IObserver *o : observers) {
    o->Update(this);
  }
}
