#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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

// functions:
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

unsigned int load_texture(const char *file, const unsigned short jpg);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

/* Settings: */
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
bool rotateCube = false;
bool rotateLight = false;
bool rgbLight = false;

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
  float vertices[] = {
    /* position       */ /* texture */ /* normal */
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
  };
  // clang-format on

  /* Shader Program: */
  Shader litShader("res/lighting/lit.vert", "res/lighting/litFlashLight.frag");
  Shader lightSourceShader("res/lighting/lightSource.vert",
                           "res/lighting/lightSource.frag");

  /* Buffer Objects: */
  VAO cubeVao;
  VBO vbo(vertices, sizeof(vertices));
  VAO lightVao;

  cubeVao.Bind();
  // ebo.Bind();

  // position attribute:
  cubeVao.LinkVBO(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);

  // texture attribute:
  cubeVao.LinkVBO(vbo, 1, 2, GL_FLOAT, 8 * sizeof(float),
                  (void *)(3 * sizeof(float)));

  /* normal attribute: */
  cubeVao.LinkVBO(vbo, 2, 3, GL_FLOAT, 8 * sizeof(float),
                  (void *)(5 * sizeof(float)));

  /* Loading textures: */
  Texture texContainer("res/container2.png", GL_TEXTURE_2D, GL_TEXTURE0,
                       GL_RGBA, GL_UNSIGNED_BYTE);
  Texture texSpecular("res/container2_specular.png", GL_TEXTURE_2D, GL_TEXTURE1,
                      GL_RGBA, GL_UNSIGNED_BYTE);
  Texture texEmissive("res/matrix.jpg", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGB,
                      GL_UNSIGNED_BYTE);

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
      lightColor.z = sin(glfwGetTime() * 1.3f);
      lightColor.x = sin(glfwGetTime() * 2.0f);
      lightColor.y = sin(glfwGetTime() * 0.7f);
    }
    glm::vec3 diffuseColor = lightColor;
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.25f);

    litShader.setVec3("viewPos", camera.Position);

    // material:
    // litShader.setVec3("material.ambient", glm::vec3(0.3f, 0.5f, 0.31f));
    // litShader.setVec3("material.diffuse", glm::vec3(0.3f, 0.5f, 0.31f));
    // litShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    litShader.setFloat("material.shininess", 64.0f);

    // light:
    litShader.setVec3("light.ambient", ambientColor);
    litShader.setVec3("light.diffuse", diffuseColor);
    litShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    litShader.setFloat("light.cutoff", glm::cos(glm::radians(5.5f)));
    litShader.setFloat("light.outerCutoff", glm::cos(glm::radians(10.5f)));
    litShader.setFloat("light.constant", 1.0f);
    litShader.setFloat("light.linear", 0.007f);
    litShader.setFloat("light.quadratic", 0.0002f);
    float x = lightPos.x;
    float z = lightPos.z;
    if (rotateLight) {
      x = 1.0f + sin(time) * 20.0f;
      // z = 1.0f + cos(time) * 2.0f;
    }
    // litShader.setVec4("light.position", glm::vec4(x, lightPos.y, z, 1.0f));
    litShader.setVec4("light.position", glm::vec4(camera.Position, 1.0f));
    litShader.setVec3("light.direction", glm::vec3(camera.Front));

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
    // lightSourceShader.use();
    // model = glm::mat4(1.0f);
    // // lightPos.z += cos(time) * radius;
    // model = glm::translate(model, glm::vec3(x, lightPos.y, z));
    // model = glm::scale(model, glm::vec3(0.2f));
    //
    // lightSourceShader.setVec3("lightColor", lightColor);
    // lightSourceShader.setMat4("model", model);
    // lightSourceShader.setMat4("view", view);
    // lightSourceShader.setMat4("projection", projection);
    //
    // lightVao.Bind();
    // glDrawArrays(GL_TRIANGLES, 0, 36);

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
