#pragma once

#include "texture.hpp"
#include <IDrawStrategy.hpp>
#include <LightSource.hpp>
#include <vector>

class DefaultDrawStrategy : public IDrawStrategy {
public:
  void Draw(RenderContext &context, Mesh &mesh) override;

private:
  void bindTextures(Shader &shader, std::vector<Texture> textures,
                    unsigned int &numDiffuse, unsigned int &numSpecular,
                    unsigned int &numEmissive);
};
