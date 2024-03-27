#pragma once

#include <vector>
#include "LightSource.hpp"
#include "camera.hpp"
// #include "lightSource.hpp"
// #include "model.hpp"
#include "s_Matrices.hpp"
#include "s_MouseInput.hpp"
#include "texture.hpp"

class RenderContext {
private:
  Camera camera;
  s_MouseInput mouse;
  s_Matrices matrices;
  glm::vec3 bgColor;
  Shader shader;
  // Texture texture;
  std::vector<LightSource *> lightSources;

public:
  RenderContext(Shader &sha);

  void setCamera(Camera &camera);
  void setMouseInput(s_MouseInput &mouse);
  void setMatrices(s_Matrices &matrices);
  void setModelMatrix(glm::mat4 &modelMatrix);
  void setBgColor(glm::vec3 bgColor);
  void setShader(Shader &shader);
  // void setTexture(Texture &texture);
  void setLightSources(std::vector<LightSource *> &lightSrcs);

  Camera &getCamera();
  s_MouseInput &getMouseInput();
  s_Matrices &getMatrices();
  glm::vec3 &getBgColor();
  Shader &getShader();
  // Texture getTexture();
  std::vector<LightSource *> &getLightSources();
};
