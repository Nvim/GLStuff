#pragma once

class LightSource;

class IObserver {
public:
  virtual ~IObserver() {}
  virtual void Update(LightSource *lightSource) = 0;
};
