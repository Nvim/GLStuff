#include "mesh.hpp"
#include "lightSource.hpp"
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

void Mesh::Draw(Shader &shader, Camera &camera, glm::mat4 model,
                Texture *texture) {
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

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  projection =
      glm::perspective(glm::radians(camera.Zoom),
                       float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
  view = camera.GetViewMatrix();

  // matrices:
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.setVec3("viewPos", camera.Position);

  // material:
  shader.setFloat("material.shininess", 32.0f);

  // dir light:
  shader.setVec3("dirLight.direction", glm::vec3(0.2f, -1.0f, -0.3f));
  shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
  shader.setVec3("dirLight.diffuse", glm::vec3(0.3f));
  shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

  // point lights
  for (int i = 0; i < 4; i++) {
    // if (rotateLight) {
    //   x += 1.0f + sin(time) * 20.0f;
    //   z += 1.0f + cos(time) * 2.0f;
    // }
    shader.setVec3("pointLights[" + std::to_string(i) + "].ambient",
                   lightSources[i].lightSettings.ambient);
    shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",
                   lightSources[i].lightSettings.diffuse);
    shader.setVec3("pointLights[" + std::to_string(i) + "].specular",
                   lightSources[i].lightSettings.specular);
    shader.setVec3("pointLights[" + std::to_string(i) + "].position",
                   pointLightPositions[i]);
    shader.setFloat("pointLights[" + std::to_string(i) + "].constant",
                    lightSources[i].lightSettings.constant);
    shader.setFloat("pointLights[" + std::to_string(i) + "].linear",
                    lightSources[i].lightSettings.linear);
    shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic",
                    lightSources[i].lightSettings.quadratic);
  }
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  // glDrawArrays(GL_TRIANGLES, 0, 36);
  vao.Unbind();
}
