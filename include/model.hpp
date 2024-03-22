#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <vector>
#include "IDrawStrategy.hpp"
#include "IObserver.hpp"
#include "RenderContext.hpp"
#include "mesh.hpp"
#include "s_LightSettings.hpp"

class Model {
public:
  Model(const char *path);
  void Draw(RenderContext &context);
  void setAsLightSource(s_LightSettings lightSettings);
  void loadModel();
  void loadModelVerbose();
  void setDrawStrategy(IDrawStrategy &strategy);
  void setLightSourcesList(std::vector<LightSource *>);

private:
  IDrawStrategy *DrawStrategy;
  std::string path;
  std::string directory;
  std::vector<Mesh> meshes;
  std::vector<Texture> textures_loaded;
  std::vector<Texture> customTextures;
  std::vector<LightSource *> lightSources;

  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
};
