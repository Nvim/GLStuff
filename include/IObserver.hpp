#pragma once

#include "glm/fwd.hpp"

class IObserver {
public:
  virtual ~IObserver() {}
  virtual void Update(const glm::mat4 position) = 0;
};
