#pragma once

#include "IObserver.hpp"
#include "LightSource.hpp"
#include "RenderContext.hpp"
#include "model.hpp"

class Scene : public IObserver {
private:
  std::vector<LightSource *> lightSourcesList;
  std::vector<Model *> models;

public:
  void addModel(Model *model);
  void addLightSource(LightSource *lightSource);
  void removeLightSource(LightSource *lightSrc);
  void Update(LightSource *lightSource) override;
  void updateModelLightSourcesList();
  void updateContextLightSourcesList(RenderContext &context);
};
