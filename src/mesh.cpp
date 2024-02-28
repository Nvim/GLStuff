#include "mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  vao.Bind();
  VBO vbo(vertices);
  EBO ebo(indices);

  // position attribute:
  vao.LinkVBO(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);

  // texture attribute:
  vao.LinkVBO(vbo, 1, 2, GL_FLOAT, sizeof(Vertex),
              (void *)offsetof(Vertex, TexCoords));

  /* normal attribute: */
  // vao.LinkVBO(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(5 *
  // sizeof(float)));
  vao.LinkVBO(vbo, 2, 3, GL_FLOAT, sizeof(Vertex),
              (void *)offsetof(Vertex, Normal));

  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
}

void Mesh::Draw(Shader &shader, Camera &camera) {
  shader.use();
  vao.Bind();

  unsigned int numDiffuse = 0;
  unsigned int numSpecular = 0;
  unsigned int numEmissive = 0;

  for (unsigned int i = 0; i < textures.size(); i++) {
    std::string num;
    std::string type = textures[i].type;

    if (type == "diffuse") {
      num = std::to_string(numDiffuse++);
    } else if (type == "emissive") {
      num = std::to_string(numEmissive++);
    } else {
      num = std::to_string(numSpecular++);
    }

    textures[i].TexUnit(shader, ("material." + type + num).c_str(), i);
    textures[i].Bind();
  }
  // TODO: Camera matrces uniforms here?
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.Unbind();
}
