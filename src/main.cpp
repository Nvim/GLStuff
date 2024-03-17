#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "lightSource.hpp"
#include "lighting.hpp"
#include "scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <vao.hpp>
#include <vbo.hpp>
#include <ebo.hpp>
#include <mesh.hpp>
#include <texture.hpp>
#include "model.hpp"

// functions:
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

unsigned int load_texture(const char *file, const unsigned short jpg);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Scene &scene);

/* Settings: */
bool rotateCube = false;
bool rotateLight = false;
bool rgbLight = false;

/* Camera: */
// Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));

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

  /* Shader Program: */
  Shader litShader("res/lighting/lit.vert", "res/lighting/litFinal.frag");
  Shader lightSourceShader("res/lighting/lightSource.vert",
                           "res/lighting/lightSource.frag");

  std::string path_backpack("res/backpack/backpack.obj");
  std::string path_cube("res/cube/cube.obj");
  float t1 = glfwGetTime();
  Model backpack(path_backpack.c_str());
  backpack.loadModel(path_backpack.c_str());
  float t2 = glfwGetTime();

  Model cube(path_cube.c_str());
  cube.loadModel(path_cube.c_str());

  std::cout << "Model loaded in " << t2 - t1 << " s." << std::endl;

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glm::mat4 model = glm::mat4(1.0f);

  s_lightSettings defaultLightSettings;
  defaultLightSettings.ambient = glm::vec3(0.1f, 0.25f, 0.2f);
  defaultLightSettings.diffuse = glm::vec3(0.2f, 0.5f, 0.4f);
  defaultLightSettings.specular = glm::vec3(1.0f, 1.0f, 1.0f);
  defaultLightSettings.constant = 1.0f;
  defaultLightSettings.linear = 0.007f;
  defaultLightSettings.quadratic = 0.0002f;

  std::vector<lightSource> lightSources;
  for (int i = 0; i < 4; i++) {
    lightSources.push_back(
        lightSource("res/backpack/backpack.obj", defaultLightSettings));
  }

  std::vector<Model> models;
  models.reserve(2);
  models.push_back(backpack);
  models.push_back(cube);

  Scene scene(models, lightSources);
  scene.setBgColor(glm::vec3(0.0f, 0.1f, 0.24f));
  glfwSetWindowUserPointer(window, &scene);

  /* ***************************************************************** */
  /* Game loop: */
  /* ***************************************************************** */

  while (!glfwWindowShouldClose(window)) {
    float x = lightPos.x;
    float z = lightPos.z;
    // timing & input:
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float time = (float)glfwGetTime();
    processInput(window, scene);

    glClearColor(scene.bgColor.x, scene.bgColor.y, scene.bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pointLightPositions[0] = glm::vec3(-5, 5, -5);
    // if (rotateLight) {
    //   x += 1.0f + sin(time) * 15.0f;
    //   z += 1.0f + cos(time) * 15.0f;
    //   pointLightPositions[0] = glm::vec3(x, lightPos.y, z);
    // }

    // glm::vec3 lightColor(1.0f);
    // if (rgbLight) {
    //   lightColor.x = fmax(sin(glfwGetTime() * 2.0f), 0.1f);
    //   lightColor.y = fmax(cos(glfwGetTime() * 0.7f), 0.1f);
    //   lightColor.z = fmax(sin(glfwGetTime() * 1.3f), 0.1f);
    // }

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
    if (rotateCube) {
      model = glm::rotate(model, glm::radians(45.0f) * time,
                          glm::vec3(0.0f, 1.0f, 0.0f));
      defaultLightSettings.diffuse.x = fmax(sin(glfwGetTime() * 2.0f), 0.1f);
      for (auto source : lightSources) {
        source.setLightSettings(defaultLightSettings);
      }
    }
    backpack.Draw(litShader, camera, model);

    for (int i = 0; i < 4; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.6f));
      cube.Draw(lightSourceShader, camera, model);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup:
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

void processInput(GLFWwindow *window, Scene &scene) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    scene.camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    scene.camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    scene.camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    scene.camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    glfwSetCursorPosCallback(window, mouse_callback);
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    rotateCube == true ? rotateCube = false : rotateCube = true;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    rotateLight == true ? rotateLight = false : rotateLight = true;
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    rgbLight == true ? rgbLight = false : rgbLight = true;
  if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    scene.dirLight == true ? scene.dirLight = false : scene.dirLight = true;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    scene.camera.ProcessArrows(DIR_UP, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    scene.camera.ProcessArrows(DIR_DOWN, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    scene.camera.ProcessArrows(DIR_LEFT, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    scene.camera.ProcessArrows(DIR_RIGHT, 1.0f);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  Scene *scene = (Scene *)glfwGetWindowUserPointer(window);
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (scene->firstMouse) // initially set to true
  {
    scene->lastMouseX = xpos;
    scene->lastMouseY = ypos;
    scene->firstMouse = false;
  }

  float xOffset = (xpos - scene->lastMouseX);
  float yOffset = (scene->lastMouseY - ypos);
  scene->lastMouseX = xpos;
  scene->lastMouseY = ypos;

  scene->camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Scene *scene = (Scene *)glfwGetWindowUserPointer(window);
  scene->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
