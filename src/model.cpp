#include "DefaultDrawStrategy.hpp"
#include "LightSourceDrawStrategy.hpp"
#include <assimp/material.h>
#include <model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Model::Model(const char *path) : modelMatrix(1.0f) { this->path = path; }

void Model::Draw(RenderContext &context) {
  context.setModelMatrix(modelMatrix);
  for (Mesh &m : meshes) {
    DrawStrategy->Draw(context, m);
  }
}

void Model::loadModelVerbose() {
  float t1 = glfwGetTime();
  loadModel();
  float t2 = glfwGetTime();
  std::cout << "Model loaded in " << t2 - t1 << " secs" << std::endl;
}

void Model::unsetLightSource() { this->DrawStrategy = new DefaultDrawStrategy; }

void Model::setDrawStrategy(IDrawStrategy &strategy) {
  this->DrawStrategy = &strategy;
}

void Model::setAsLightSource(s_LightSettings &lightSettings) {
  this->DrawStrategy = new LightSourceDrawStrategy(lightSettings);
}

void Model::loadModel() {
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
  std::cout << "Model loaded!" << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int j = 0; j < node->mNumChildren; j++) {
    processNode(node->mChildren[j], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  // collect all vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;
    // position
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.Position = vector;
    // normals:
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
    }
    // texture coordinates:
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      // TODO: Tangents and BiTangents
      vertex.TexCoords = vec;
    } else {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }
    vertices.push_back(vertex);
  }

  // collect all indices:
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // get material info if set:
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // get all diffuse textures and insert them in tex vector with proper names:
    std::vector<Texture> specularMaps =
        loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //
    std::vector<Texture> diffuseMaps =
        loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // TODO: Height and NormalMaps
  }

  for (auto tex : customTextures) {
    textures.push_back(tex);
    std::cout << "loaded a custom texture" << std::endl;
  }

  return Mesh(vertices, indices, textures);
}

// TODO: look at texturefromfile for path concat
std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName) {
  std::vector<Texture> textures;
  unsigned int i;
  for (i = 0; i < mat->GetTextureCount(type); i++) {
    aiString texName;
    mat->GetTexture(type, i, &texName);
    std::string file = std::string(texName.data, texName.length);
    std::string filepath = directory + "/" + file;
    bool skip = false;

    // check if it's already loaded using path:
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
      if (std::strcmp(textures_loaded[j].path.c_str(), filepath.c_str()) == 0) {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }

    if (!skip) {
      const char *textureType = typeName.c_str();
      const char *texturePath = filepath.c_str();

      Texture texture(texturePath, (textureType + std::to_string(i)).c_str(),
                      GL_TEXTURE0 + i);
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  // numTextures = i;
  return textures;
}

// observer stuff:
std::vector<IObserver *> &Model::getObservers() { return observers; }

void Model::AddObserver(IObserver *o) { observers.push_back(o); }

void Model::RemoveObserver(IObserver *o) {
  observers.erase(std::remove(observers.begin(), observers.end(), o),
                  observers.end());
}

void Model::NotifyObservers() {
  for (IObserver *o : observers) {
    o->Update(this->modelMatrix);
  }
}

const glm::mat4 &Model::getModelMatrix() const { return modelMatrix; }

void Model::rotate(float angle, const glm::vec3 &axis) {
  modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
  NotifyObservers();
}

void Model::translate(const glm::vec3 &translation) {
  modelMatrix = glm::translate(modelMatrix, translation);
  NotifyObservers();
}

void Model::scale(const glm::vec3 &scale) {
  modelMatrix = glm::scale(modelMatrix, scale);
  NotifyObservers();
}

void Model::resetModelMatrix() {
  modelMatrix = glm::mat4(1.0f);
  NotifyObservers();
}
