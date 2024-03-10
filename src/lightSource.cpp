#include <lightSource.hpp>

void lightSource::Draw(Shader &shader, Camera &camera, glm::mat4 model,
                       Texture *texture) {

  shader.setVec3("lightColor", lightSettings.ambient);
  Model::Draw(shader, camera, model, texture);
}

void lightSource::setLightSettings(s_lightSettings &light) {
  lightSettings = light;
}
