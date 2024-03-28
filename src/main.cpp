#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "DefaultDrawStrategy.hpp"
#include "Scene.hpp"
#include "s_Matrices.hpp"
#include "s_MouseInput.hpp"
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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
void processInput(GLFWwindow *window, RenderContext &context);

/* Settings: */
bool rotateCube = false;
bool rotateLight = false;
bool rgbLight = false;

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

  /* Models */
  Model backpack("res/backpack/backpack.obj");
  Model cube("res/cube/cube.obj");
  backpack.loadModelVerbose();
  cube.loadModel();

  /* Matrices */
  s_Matrices matrices;
  matrices.model = glm::mat4(1.0f);
  matrices.view = glm::mat4(1.0f);
  matrices.projection = glm::mat4(1.0f);

  /* LightSettings */
  s_LightSettings defaultLightSettings;
  defaultLightSettings.ambient = glm::vec3(0.5f, 0.1f, 0.1f);
  defaultLightSettings.diffuse = glm::vec3(1.0f, 0.2f, 0.2f);
  defaultLightSettings.specular = glm::vec3(1.0f, 1.0f, 1.0f);
  defaultLightSettings.constant = 1.0f;
  defaultLightSettings.linear = 0.007f;
  defaultLightSettings.quadratic = 0.0002f;

  /* Camera: */
  Camera camera(glm::vec3(0.0f, 0.0f, 17.0f));
  s_MouseInput mouseInput;

  RenderContext context(litShader);
  context.setShader(litShader);
  context.setCamera(camera);
  context.setMouseInput(mouseInput);
  context.setMatrices(matrices);
  context.setBgColor(glm::vec3(0.0f, 0.1f, 0.24f));

  DefaultDrawStrategy defaultStrat;
  backpack.setDrawStrategy(defaultStrat);
  cube.setDrawStrategy(defaultStrat);
  cube.translate(glm::vec3(0.0f, 0.0f, 0.0f));
  cube.scale(glm::vec3(0.5f, 0.5f, 0.5f));

  context.addLightSource(cube, defaultLightSettings);
  defaultLightSettings.position = cube.getModelMatrix()[3];
  // cube.setAsLightSource(defaultLightSettings);

  glfwSetWindowUserPointer(window, &context);
  std::cout << "Entering game loop" << std::endl;

  const float radius = 2.0f;
  const float rotationSpeed = 0.4f;

  /* ***************************************************************** */
  /* Game loop: */
  /* ***************************************************************** */

  while (!glfwWindowShouldClose(window)) {
    // timing & input:
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float time = (float)glfwGetTime();
    float angle = time * rotationSpeed; // rotation angle
    processInput(window, context);

    glm::vec3 bg = context.getBgColor();
    glClearColor(bg.x, bg.y, bg.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    matrices.projection =
        glm::perspective(glm::radians(context.getCamera().Zoom),
                         float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
    matrices.view = context.getCamera().GetViewMatrix();
    context.setMatrices(matrices);

    float x = 1.0f + sin(time) * radius;
    float y = sin(time / 2.0f) * 1.0f;
    cube.resetModelMatrix();
    cube.translate(glm::vec3(-9.0f, 0.0f, 9.0f));
    cube.translate(glm::vec3(x, y, 0.0f));
    defaultLightSettings.ambient.x += sin(time);
    defaultLightSettings.ambient.y += cos(time) / 2.0f;
    defaultLightSettings.ambient.z += sin(time) * 2.0f;

    backpack.Draw(context);
    cube.Draw(context);

    // std::cout << "Player position: (" << context.getCamera().Position.x << ",
    // "
    //           << context.getCamera().Position.y << ", "
    //           << context.getCamera().Position.z << ")" << std::endl;
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

void processInput(GLFWwindow *window, RenderContext &context) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    glfwSetCursorPosCallback(window, mouse_callback);
  // if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
  //   scene.dirLight == true ? scene.dirLight = false : scene.dirLight = true;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    context.getCamera().ProcessArrows(DIR_UP, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    context.getCamera().ProcessArrows(DIR_DOWN, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    context.getCamera().ProcessArrows(DIR_LEFT, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    context.getCamera().ProcessArrows(DIR_RIGHT, 1.0f);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  RenderContext *context = (RenderContext *)glfwGetWindowUserPointer(window);
  s_MouseInput in = context->getMouseInput();
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (in.firstMouse) // initially set to true
  {
    in.lastMouseX = xpos;
    in.lastMouseY = ypos;
    in.firstMouse = false;
  }

  float xOffset = (xpos - in.lastMouseX);
  float yOffset = (in.lastMouseY - ypos);
  in.lastMouseX = xpos;
  in.lastMouseY = ypos;

  context->getCamera().ProcessMouseMovement(xOffset, yOffset);
  context->setMouseInput(in);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  RenderContext *context = (RenderContext *)glfwGetWindowUserPointer(window);
  context->getCamera().ProcessMouseScroll(static_cast<float>(yoffset));
}
