#pragma once

#include "IObserver.hpp"
#include "ISubject.hpp"
#include <vector>
#include "s_LightSettings.hpp"

class LightSource : public ISubject {
private:
  s_LightSettings lightSettings;
  std::vector<IObserver *> observers;

public:
  void setLightSettings(s_LightSettings &light);
  s_LightSettings getLightSettings();

  void RegisterObserver(IObserver *observer) override;
  void RemoveObserver(IObserver *observer) override;
  void NotifyObservers() override;
};
