#pragma once

#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <mesh.hpp>

class Model {
public:
  int numTextures = 0; // to know which slot to add an external tex
  Model(const char *path);
  void Draw(Shader &shader, Camera &camera, glm::mat4 model,
            glm::vec3 lightColor, Texture *texture = nullptr);
  std::vector<Texture> textures_loaded;
  std::vector<Texture> customTextures;

  void addCustomTexture(const char *path, const char *type) {
    Texture customTex(path, type, GL_TEXTURE0);
    // add custom textures if any:
    customTextures.push_back(customTex);
    std::cout << "added a custom tex:" << customTex.path
              << " - slot: " << customTex.slot << std::endl;
  }
  void loadModel(std::string path);

private:
  std::vector<Mesh> meshes;
  std::string directory;

  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
};
