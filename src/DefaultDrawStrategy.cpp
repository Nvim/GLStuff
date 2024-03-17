#include "lightSource.hpp"
#include <IDrawStrategy.hpp>

class DefaultDrawStrategy : public IDrawStrategy {
public:
  void Draw(RenderContext &context, Mesh &mesh) override {

    Shader shader = context.getShader();
    VAO vao = mesh.vao;
    std::vector<unsigned int> indices = mesh.indices;
    std::vector<Texture> textures = mesh.textures;
    Camera camera = context.getCamera();
    vao.Bind();
    std::vector<lightSource> lights = context.getLights();

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

    // glm::mat4 view = glm::mat4(1.0f);
    // glm::mat4 projection = glm::mat4(1.0f);
    // projection =
    //     glm::perspective(glm::radians(camera.Zoom),
    //                      float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
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
      // if (rotateLight) {
      //   x += 1.0f + sin(time) * 20.0f;
      //   z += 1.0f + cos(time) * 2.0f;
      // }
      shader.setVec3("pointLights[" + std::to_string(i) + "].ambient",
                     lights[i].lightSettings.ambient);
      shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",
                     lights[i].lightSettings.diffuse);
      shader.setVec3("pointLights[" + std::to_string(i) + "].specular",
                     lights[i].lightSettings.specular);
      shader.setVec3("pointLights[" + std::to_string(i) + "].position",
                     lights[i].lightSettings.position);
      shader.setFloat("pointLights[" + std::to_string(i) + "].constant",
                      lights[i].lightSettings.constant);
      shader.setFloat("pointLights[" + std::to_string(i) + "].linear",
                      lights[i].lightSettings.linear);
      shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic",
                      lights[i].lightSettings.quadratic);
    }
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.Unbind();
  }
};
