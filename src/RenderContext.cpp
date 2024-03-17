#include <RenderContext.hpp>

// setters:
void RenderContext::setCamera(Camera &camera) { this->camera = camera; }
void RenderContext::setModels(std::vector<Model> &models) {
  this->models = models;
}
void RenderContext::setMouseInput(s_MouseInput &mouse) { this->mouse = mouse; }
void RenderContext::setMatrices(s_Matrices &matrices) {
  this->matrices = matrices;
}
void RenderContext::setBgColor(glm::vec3 bgColor) { this->bgColor = bgColor; }
void RenderContext::setShader(Shader &shader) { this->shader = shader; }
void RenderContext::setTexture(Texture &texture) { this->texture = texture; }

// getters:
Camera RenderContext::getCamera() { return this->camera; }
std::vector<Model> RenderContext::getModels() { return this->models; }
s_MouseInput RenderContext::getMouseInput() { return this->mouse; }
s_Matrices RenderContext::getMatrices() { return this->matrices; }
glm::vec3 RenderContext::getBgColor() { return this->bgColor; }
Shader RenderContext::getShader() { return this->shader; }
Texture RenderContext::getTexture() { return this->texture; }
std::vector<lightSource> RenderContext::getLights() { return this->lights; }
