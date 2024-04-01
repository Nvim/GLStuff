#pragma once

#include <vector>
#include "LightSource.hpp"
#include "camera.hpp"
// #include "lightSource.hpp"
// #include "model.hpp"
#include "s_Matrices.hpp"
#include "s_MouseInput.hpp"

class Model;

class RenderContext {
private:
  Camera camera;
  s_MouseInput mouse;
  s_Matrices matrices;
  glm::vec3 bgColor;
  Shader shader;
  std::vector<LightSource *> lightSources;

  s_DirLightSettings dirLightSettings;
  bool useDirLight;

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
  void addLightSource(Model &m, LightSource &ls);
  void addLightSource(Model &m, s_LightSettings &ls);
  void RemoveLightSource(Model &m);

  void enableDirLight();
  void disableDirLight();
  void setDirLightSettings(s_DirLightSettings &ls);
  bool getDirLightStatus();
  s_DirLightSettings &getDirLight();

  Camera &getCamera();
  s_MouseInput &getMouseInput();
  s_Matrices &getMatrices();
  glm::vec3 &getBgColor();
  Shader &getShader();
  // Texture getTexture();
  std::vector<LightSource *> &getLightSources();
};
