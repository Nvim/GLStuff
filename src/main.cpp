#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
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

  unsigned int indicies[] = {
    1, 2, 3, 4, 1, 3, 4, 9
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

  std::vector<Texture> tex(std::begin(textures), std::end(textures));
  std::vector<Vertex> verts(std::begin(vertices), std::end(vertices));
  std::vector<unsigned int> ind(std::begin(indicies), std::end(indicies));

  Mesh container = Mesh(verts, ind, tex);
  Mesh light = Mesh(verts, ind, tex);

  float x = lightPos.x;
  float z = lightPos.z;
  /* Game loop: */
  while (!glfwWindowShouldClose(window)) {
    // timing & input:
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float time = (float)glfwGetTime();
    processInput(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightColor(1.0f);
    if (rgbLight) {
      lightColor.x = fmax(sin(glfwGetTime() * 2.0f), 0.1f);
      lightColor.y = fmax(cos(glfwGetTime() * 0.7f), 0.1f);
      lightColor.z = fmax(sin(glfwGetTime() * 1.3f), 0.1f);
    }

    /* draw lit cube: */
    container.Draw(litShader, camera, lightColor);
    for (int i = 0; i < 4; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f));
      light.DrawLight(lightSourceShader, camera, model, lightColor);
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
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camera.ProcessArrows(DIR_UP, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camera.ProcessArrows(DIR_DOWN, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    camera.ProcessArrows(DIR_LEFT, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    camera.ProcessArrows(DIR_RIGHT, 1.0f);
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

  float xOffset = (xpos - lastMouseX);
  float yOffset = (lastMouseY - ypos);
  lastMouseX = xpos;
  lastMouseY = ypos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
