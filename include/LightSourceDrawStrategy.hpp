#pragma once

#include "s_LightSettings.hpp"
#include "texture.hpp"
#include <IDrawStrategy.hpp>
#include <LightSource.hpp>
#include <vector>

class LightSourceDrawStrategy : public IDrawStrategy {
public:
  LightSourceDrawStrategy(s_LightSettings lightSettings);
  void Draw(RenderContext &context, Mesh &mesh) override;
  void getLightSettings();
  void setLightSettings(s_LightSettings &lightSettings);

private:
  s_LightSettings lightSettings;
  void bindTextures(Shader &shader, std::vector<Texture> textures,
                    unsigned int &numDiffuse, unsigned int &numSpecular,
                    unsigned int &numEmissive);
};
