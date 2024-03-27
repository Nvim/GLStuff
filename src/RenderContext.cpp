#include "texture.hpp"
#include <RenderContext.hpp>
#include <iostream>

// setters:
RenderContext::RenderContext(Shader &sha) : shader(sha) {
  std::cout << "Context Created" << std::endl;
}

void RenderContext::setCamera(Camera &camera) { this->camera = camera; }
void RenderContext::setMouseInput(s_MouseInput &mouse) { this->mouse = mouse; }
void RenderContext::setMatrices(s_Matrices &matrices) {
  this->matrices = matrices;
}
void RenderContext::setModelMatrix(glm::mat4 &modelMatrix) {
  this->matrices.model = modelMatrix;
}
void RenderContext::setBgColor(glm::vec3 bgColor) { this->bgColor = bgColor; }
void RenderContext::setShader(Shader &shader) { this->shader = shader; }
// void RenderContext::setTexture(Texture &texture) { this->texture = texture; }
void RenderContext::setLightSources(std::vector<LightSource *> &lightSrcs) {
  this->lightSources = lightSrcs;
}

// getters:
Camera &RenderContext::getCamera() { return this->camera; }
s_MouseInput &RenderContext::getMouseInput() { return this->mouse; }
s_Matrices &RenderContext::getMatrices() { return this->matrices; }
glm::vec3 &RenderContext::getBgColor() { return this->bgColor; }
Shader &RenderContext::getShader() { return this->shader; }
// Texture RenderContext::getTexture() { return this->texture; }
std::vector<LightSource *> &RenderContext::getLightSources() {
  return this->lightSources;
}
