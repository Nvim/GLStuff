#pragma once

#include <vector>
#include "camera.hpp"
// #include "lightSource.hpp"
// #include "model.hpp"
#include "s_Matrices.hpp"
#include "s_MouseInput.hpp"
#include "texture.hpp"

class RenderContext {
private:
  Camera camera;
  // std::vector<Model> models;
  s_MouseInput mouse;
  s_Matrices matrices;
  glm::vec3 bgColor;
  Shader shader;
  Texture texture;
  // std::vector<lightSource> lights;

public:
  RenderContext();

  void setCamera(Camera &camera);
  // void setModels(std::vector<Model> &models);
  // void setLights(std::vector<lightSource> &lights);
  void setMouseInput(s_MouseInput &mouse);
  void setMatrices(s_Matrices &matrices);
  void setBgColor(glm::vec3 bgColor);
  void setShader(Shader &shader);
  void setTexture(Texture &texture);

  Camera getCamera();
  // std::vector<Model> getModels();
  // std::vector<lightSource> getLights();
  s_MouseInput getMouseInput();
  s_Matrices getMatrices();
  glm::vec3 getBgColor();
  Shader getShader();
  Texture getTexture();
};
