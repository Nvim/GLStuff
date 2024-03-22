#include <LightSourceDrawStrategy.hpp>

void LightSourceDrawStrategy::Draw(RenderContext &context, Mesh &mesh) {

  Shader shader = context.getShader();
  VAO vao = mesh.vao;
  std::vector<unsigned int> indices = mesh.indices;
  std::vector<Texture> textures = mesh.textures;
  Camera camera = context.getCamera();
  vao.Bind();
  std::vector<LightSource *> lights = context.getLightSources();

  unsigned int numDiffuse = 0;
  unsigned int numSpecular = 0;
  unsigned int numEmissive = 0;

  bindTextures(shader, textures, numDiffuse, numSpecular, numEmissive);

  // glm::mat4 view = glm::mat4(1.0f);
  // glm::mat4 projection = glm::mat4(1.0f);
  // projection =
  //     glm::perspective(glm::radians(camera.Zoom),
  //                      float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f,
  //                      100.0f);
  // view = camera.GetViewMatrix();

  // matrices:
  shader.setMat4("model", context.getMatrices().model);
  shader.setMat4("view", context.getMatrices().view);
  shader.setMat4("projection", context.getMatrices().projection);
  shader.setVec3("viewPos", camera.Position);

  // material:
  shader.setFloat("material.shininess", 32.0f);

  // dir light:
  shader.setVec3("dirLight.direction", glm::vec3(0.2f, -1.0f, -0.3f));
  shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
  shader.setVec3("dirLight.diffuse", glm::vec3(0.3f));
  shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

  // point lights
  for (int i = 0; i < lights.size(); i++) {
    s_LightSettings ls = lights[i]->getLightSettings();
    shader.setVec3("pointLights[" + std::to_string(i) + "].ambient",
                   ls.ambient);
    shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",
                   ls.diffuse);
    shader.setVec3("pointLights[" + std::to_string(i) + "].specular",
                   ls.specular);
    shader.setVec3("pointLights[" + std::to_string(i) + "].position",
                   ls.position);
    shader.setFloat("pointLights[" + std::to_string(i) + "].constant",
                    ls.constant);
    shader.setFloat("pointLights[" + std::to_string(i) + "].linear", ls.linear);
    shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic",
                    ls.quadratic);
  }
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.Unbind();
}

void LightSourceDrawStrategy::bindTextures(Shader &shader,
                                           std::vector<Texture> textures,
                                           unsigned int &numDiffuse,
                                           unsigned int &numSpecular,
                                           unsigned int &numEmissive) {

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
}
