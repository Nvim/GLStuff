#include <RenderContext.hpp>
#include <iostream>
#include <model.hpp>

// setters:
RenderContext::RenderContext(Shader &sha) : shader(sha) {
  this->useDirLight = false;
  this->dirLightSettings = s_DirLightSettings();
  std::cout << "Context Created" << std::endl;
}

void RenderContext::enableDirLight() { this->useDirLight = true; }
void RenderContext::disableDirLight() { this->useDirLight = false; }

void RenderContext::setDirLightSettings(s_DirLightSettings &ls) {
  dirLightSettings = ls;
}
void RenderContext::setDirLightStatus(bool status) { useDirLight = status; }
bool RenderContext::getDirLightStatus() { return useDirLight; }

s_DirLightSettings *RenderContext::getDirLight() { return &dirLightSettings; }

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

void RenderContext::addLightSource(Model &m, LightSource &ls) {
  m.setAsLightSource(*ls.getLightSettings());
  m.AddObserver(&ls);
  ls.Update(m.getModelMatrix());
  this->lightSources.push_back(&ls);
}

void RenderContext::addLightSource(Model &m, s_LightSettings &ls) {
  LightSource *lsrc = new LightSource(ls);
  m.setAsLightSource(*lsrc->getLightSettings());
  m.AddObserver(lsrc);
  m.NotifyObservers();
  this->lightSources.push_back(lsrc);
}

void RenderContext::RemoveLightSource(Model &m) {
  /* get all lightSources tied to the Model
  and remove them from the lightSources list */
  std::vector<IObserver *> mLightSources = m.getObservers();
  for (IObserver *o : mLightSources) {
    LightSource *ls = dynamic_cast<LightSource *>(o);
    if (ls) {
      auto it = std::remove(lightSources.begin(), lightSources.end(), ls);
      if (it != lightSources.end()) {
        lightSources.erase(it, lightSources.end());
        delete ls;
      }
    }
  }
  // change model strategy
  m.unsetLightSource();
}

void RenderContext::addModel(Model &m) { this->models.push_back(&m); }

void RenderContext::addModelByName(std::string &path) {

  std::cout << "Trying to load model " << path << "\n";
  // Model _m(path.c_str());
  Model *_m = new Model(path.c_str());
  _m->loadModelVerbose();
  this->models.push_back(_m);
}

void RenderContext::loadModels() {
  for (Model *m : models) {
    m->loadModelVerbose();
  }
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
