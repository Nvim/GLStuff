#include "mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
  std::cout << "Creating mesh object" << std::endl;
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
  std::cout << "Mesh created!" << std::endl;
}

void Mesh::Draw(Shader &shader, Camera &camera, glm::vec3 lightColor) {
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

  glm::mat4 model = glm::mat4(1.0f);
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
  shader.setFloat("material.shininess", 64.0f);

  // dir light:
  shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
  shader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
  shader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
  shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

  // point lights
  glm::vec3 diffuseColor = lightColor;
  glm::vec3 ambientColor = diffuseColor * glm::vec3(0.25f);

  for (int i = 0; i < 4; i++) {
    // if (rotateLight) {
    //   x += 1.0f + sin(time) * 20.0f;
    //   z += 1.0f + cos(time) * 2.0f;
    // }
    shader.setVec3("pointLights[" + std::to_string(i) + "].ambient",
                   ambientColor);
    shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",
                   diffuseColor);
    shader.setVec3("pointLights[" + std::to_string(i) + "].specular",
                   glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3("pointLights[" + std::to_string(i) + "].position",
                   pointLightPositions[i]);
    shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
    shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
    shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
  }
  // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  vao.Unbind();
}

void Mesh::DrawLight(Shader &shader, Camera &camera, glm::mat4 model,
                     glm::vec3 lightColor) {
  shader.use();
  vao.Bind();
  glm::mat4 projection =
      glm::perspective(glm::radians(camera.Zoom),
                       float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
  glm::mat4 view = camera.GetViewMatrix();
  // lightPos.z += cos(time) * radius;
  // model = glm::translate(model, glm::vec3(x, lightPos.y, z));
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.setVec3("lightColor", lightColor);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  vao.Unbind();
}
