#pragma once

#include "RenderContext.hpp"
#include "mesh.hpp"

class IDrawStrategy {
public:
  virtual ~IDrawStrategy() {}
  virtual void Draw(RenderContext &context, Mesh &mesh) = 0;

protected:
  void bindTextures(Shader &shader, std::vector<Texture> textures,
                    unsigned int &numDiffuse, unsigned int &numSpecular,
                    unsigned int &numEmissive);
};
