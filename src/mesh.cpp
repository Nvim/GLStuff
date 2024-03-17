#include "mesh.hpp"
// #include "lightSource.hpp"
#include <cstddef>
#include <string>

glm::vec3 pointLightPositions[] = {
    glm::vec3(4.7f, 4.2f, 6.0f), glm::vec3(-2.3f, -3.3f, -7.0f),
    glm::vec3(-8.0f, 2.0f, -12.0f), glm::vec3(5.0f, 0.0f, -9.0f)};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
  // std::cout << "Creating mesh object" << std::endl;
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  vao.Bind();
  VBO vbo(vertices);
  EBO ebo(indices);
  ebo.Bind();

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
  // std::cout << "Mesh created!" << std::endl;
}
