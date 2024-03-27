#include <Scene.hpp>
#include <LightSource.hpp>

void Scene::addModel(Model *model) { models.push_back(model); }

void Scene::addLightSource(LightSource *lightSource) {
  lightSourcesList.push_back(lightSource);
  lightSource->RegisterObserver(this);
  updateModelLightSourcesList();
}

void Scene::removeLightSource(LightSource *lightSrc) {
  lightSourcesList.erase(
      std::remove(lightSourcesList.begin(), lightSourcesList.end(), lightSrc),
      lightSourcesList.end());
  lightSrc->RemoveObserver(this);
  updateModelLightSourcesList();
}

void Scene::Update(LightSource *lightSource) {
  // Called when a LightSource changes
  updateModelLightSourcesList();
}

void Scene::updateModelLightSourcesList() {
  for (Model *model : models) {
    model->setLightSourcesList(lightSourcesList);
  }
}

void Scene::updateContextLightSourcesList(RenderContext &context) {
  context.setLightSources(lightSourcesList);
}
