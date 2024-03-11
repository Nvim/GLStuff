#pragma once

#include "camera.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vao.hpp"
#include "ebo.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <lighting.hpp>

extern glm::vec3 pointLightPositions[];

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  VAO vao;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  void Draw(Shader &shader, Camera &camera, glm::mat4 model,
            Texture *texture = nullptr, Scene &scene);
};
