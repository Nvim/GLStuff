#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <vector>
#include "IDrawStrategy.hpp"
#include "IObserver.hpp"
#include "ISubject.hpp"
#include "RenderContext.hpp"
#include "mesh.hpp"
#include "s_LightSettings.hpp"

class Model : public ISubject {
public:
  Model(const char *path);
  void Draw(RenderContext &context);
  void setAsLightSource(s_LightSettings &lightSettings);
  void unsetLightSource();
  void loadModel();
  void loadModelVerbose();
  void setDrawStrategy(IDrawStrategy &strategy);
  IDrawStrategy *getDrawStrategy();
  const glm::mat4 &getModelMatrix() const;
  void rotate(float angle, const glm::vec3 &axis);
  void translate(const glm::vec3 &translation);
  void scale(const glm::vec3 &scale);
  void resetModelMatrix();
  glm::vec3 getCurrentScale();
  void setScale(glm::vec3 &newScale);
  glm::vec3 getCurrentTranslation();
  void setTranslation(glm::vec3 &newTranslation);
  glm::vec3 getCurrentRotation();
  void setRotation(glm::vec3 &newRotation);
  // observer stuff:
  std::vector<IObserver *> &getObservers();
  void AddObserver(IObserver *observer) override;
  void RemoveObserver(IObserver *observer) override;
  void NotifyObservers() override;
  void setName(std::string name);
  std::string getName();

private:
  std::string name;
  glm::mat4 modelMatrix;
  IDrawStrategy *DrawStrategy;
  std::string path;
  std::string directory;
  std::vector<IObserver *> observers;
  std::vector<Mesh> meshes;
  std::vector<Texture> textures_loaded;
  std::vector<Texture> customTextures;
  //
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
};
