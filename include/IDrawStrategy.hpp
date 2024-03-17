#pragma once

#include "RenderContext.hpp"
#include "mesh.hpp"

class IDrawStrategy {
public:
  virtual void Draw(RenderContext &context, Mesh &mesh) = 0;

private:
};
