#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "glm/ext/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <texture.hpp>
#include <vao.hpp>
#include <vbo.hpp>
#include <ebo.hpp>
#include <mesh.hpp>

// functions:
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

unsigned int load_texture(const char *file, const unsigned short jpg);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

/* Settings: */
bool rotateCube = false;
bool rotateLight = false;
bool rgbLight = false;
bool dirLight = false;

/* Camera: */
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/* Timing: */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/* lighting */
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

/***********************
 * Main *
 * *********************/
int main() {

  // glfw init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create a window
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "wouhou", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad init (gets opengl functions)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  /* 1st OpenGL function!
  Set viewport == window */
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // call our function when window is resized:
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);

  /*
    RENDERING !
  */

  // clang-format off
  Vertex vertices[] = {
    /* position       */ /* texture */ /* normal */
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
    Vertex{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f)}, 
    Vertex{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f)}, 
    Vertex{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2( 1.0f, 1.0f),  glm::vec3(0.0f,  0.0f, -1.0f)}, 
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2( 0.0f, 1.0f),  glm::vec3(0.0f,  0.0f, -1.0f)}, 
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2( 0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, -1.0f)}, 

    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2( 0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
    Vertex{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2( 1.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
    Vertex{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2( 1.0f, 1.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
    Vertex{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2( 1.0f, 1.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2( 0.0f, 1.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2( 0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},

    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)},

    Vertex{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},
    Vertex{glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)},

    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    Vertex{glm::vec3(0.5f, -0.5f, -0.5f ), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    Vertex{glm::vec3(0.5f, -0.5f,  0.5f ), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    Vertex{glm::vec3(0.5f, -0.5f,  0.5f ), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},

    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
    Vertex{glm::vec3(0.5f,  0.5f, -0.5f ), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
    Vertex{glm::vec3(0.5f,  0.5f,  0.5f ), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
    Vertex{glm::vec3(0.5f,  0.5f,  0.5f ), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f)}
  };
  glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
  };

  // clang-format on

  /* Shader Program: */
  Shader litShader("res/lighting/lit.vert", "res/lighting/litFinal.frag");
  Shader lightSourceShader("res/lighting/lightSource.vert",
                           "res/lighting/lightSource.frag");

  /* Loading textures: */
  Texture textures[]{
      //
      Texture("res/container2.png", "diffuse", GL_TEXTURE0, GL_RGBA,
              GL_UNSIGNED_BYTE),
      Texture("res/container2_specular.png", "specular", GL_TEXTURE1, GL_RGBA,
              GL_UNSIGNED_BYTE),
      Texture("res/matrix.jpg", "emissive", GL_TEXTURE2, GL_RGB,
              GL_UNSIGNED_BYTE) //
  };

  texContainer.TexUnit(litShader, "material.diffuse", 0);
  texContainer.TexUnit(litShader, "material.specular", 1);
  texContainer.TexUnit(litShader, "material.emissive", 2);
  cubeVao.Unbind();

  lightVao.Bind();
  lightVao.LinkVBO(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  lightVao.Unbind();

  /* Transformation Matrices: */
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  float x = lightPos.x;
  float z = lightPos.z;
  /* Game loop: */
  while (!glfwWindowShouldClose(window)) {
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    // timing & input:
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float time = (float)glfwGetTime();
    processInput(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projection =
        glm::perspective(glm::radians(camera.Zoom),
                         float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
    view = camera.GetViewMatrix();

    /* draw lit cube: */
    litShader.use();
    glm::vec3 lightColor = glm::vec3(1.0f);
    if (rgbLight) {
      lightColor.x = fmax(sin(glfwGetTime() * 2.0f), 0.1f);
      lightColor.y = fmax(cos(glfwGetTime() * 0.7f), 0.1f);
      lightColor.z = fmax(sin(glfwGetTime() * 1.3f), 0.1f);
    }
    glm::vec3 diffuseColor = lightColor;
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.25f);

    litShader.setVec3("viewPos", camera.Position);

    // material:
    litShader.setFloat("material.shininess", 64.0f);

    // lights:
    if (dirLight) {
      litShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
      litShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
      litShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
      litShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    }

    for (int i = 0; i < 4; i++) {
      // if (rotateLight) {
      //   x += 1.0f + sin(time) * 20.0f;
      //   z += 1.0f + cos(time) * 2.0f;
      // }
      litShader.setVec3("pointLights[" + std::to_string(i) + "].ambient",
                        ambientColor);
      litShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",
                        diffuseColor);
      litShader.setVec3("pointLights[" + std::to_string(i) + "].specular",
                        glm::vec3(1.0f, 1.0f, 1.0f));
      litShader.setVec3("pointLights[" + std::to_string(i) + "].position",
                        pointLightPositions[i]);
      litShader.setFloat("pointLights[" + std::to_string(i) + "].constant",
                         1.0f);
      litShader.setFloat("pointLights[" + std::to_string(i) + "].linear",
                         0.09f);
      litShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic",
                         0.032f);
    }
    // litShader.setFloat("pointLights[" + std::to_string(i) + "].cutoff",
    //                    glm::cos(glm::radians(12.0f)));
    // litShader.setFloat("pointLights[" + std::to_string(i) +
    // "].outerCutoff",
    //                    glm::cos(glm::radians(17.5f)));
    // matrices:
    litShader.setMat4("view", view);
    litShader.setMat4("projection", projection);
    if (rotateCube) {
      model = glm::rotate(model, glm::radians(45.0f) * time,
                          glm::vec3(0.0f, 1.0f, 0.0f));
    }
    litShader.setMat4("model", model);

    texContainer.Bind();
    texSpecular.Bind();
    texEmissive.Bind();
    cubeVao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    /* draw light source: */
    lightSourceShader.use();

    lightVao.Bind();
    for (int i = 0; i < 4; i++) {
      model = glm::mat4(1.0f);
      // lightPos.z += cos(time) * radius;
      // model = glm::translate(model, glm::vec3(x, lightPos.y, z));
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f));
      lightSourceShader.setMat4("model", model);
      lightSourceShader.setVec3("lightColor", lightColor);
      lightSourceShader.setMat4("view", view);
      lightSourceShader.setMat4("projection", projection);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup:
  cubeVao.Delete();
  lightVao.Delete();
  vbo.Delete();
  texContainer.Delete();
  // texHuh.Delete();
  litShader.Delete();
  lightSourceShader.Delete();
  glfwTerminate();
  return 0;
}

GLenum glCheckError_(const char *file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      error = "UNKNOWN ERROR";
      break;
    }
    std::cout << error << " | " << file << " (" << line << ")" << std::endl;
  }
  return errorCode;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    glfwSetCursorPosCallback(window, mouse_callback);
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    rotateCube == true ? rotateCube = false : rotateCube = true;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    rotateLight == true ? rotateLight = false : rotateLight = true;
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    rgbLight == true ? rgbLight = false : rgbLight = true;
  if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    dirLight == true ? dirLight = false : dirLight = true;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (firstMouse) // initially set to true
  {
    lastMouseX = xpos;
    lastMouseY = ypos;
    firstMouse = false;
  }

  float xOffset = xpos - lastMouseX;
  float yOffset = lastMouseY - ypos;
  lastMouseX = xpos;
  lastMouseY = ypos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
