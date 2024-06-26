#pragma once

#include "IObserver.hpp"

class ISubject {
public:
  ~ISubject() {}
  virtual void AddObserver(IObserver *observer) = 0;
  virtual void RemoveObserver(IObserver *observer) = 0;
  virtual void NotifyObservers() = 0;
};
