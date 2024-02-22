#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <alloca.h>
#include <iostream>
#include "glm/ext/matrix_transform.hpp"
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"

// functions:
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

unsigned int load_texture(const char *file, const unsigned short jpg);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

/* Settings: */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Camera: */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/* Timing: */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  /*
    RENDERING !
  */

  /* Shader Program: */
  Shader shader = Shader("res/shader.vert", "res/fade.frag");

  // clang-format off
  float vertices_old[] = {
     // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
  };
  unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
  };
  // clang-format on

  /* Buffer Objects: */
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO); // gen VAO 1st
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  // Binding buffers:
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // Set vertex attributes:
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)0); // position attribute
  glEnableVertexAttribArray(0);

  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* Loading textures: */
  unsigned int texture = load_texture("res/container.jpg", 1);
  unsigned int texture2 = load_texture("res/awesomeface.png", 0);
  unsigned int texture3 = load_texture("res/huh.png", 0);
  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);
  glUniform1i(glGetUniformLocation(shader.ID, "otherTexture"), 1);
  glUniform1i(glGetUniformLocation(shader.ID, "otherTexture2"), 2);

  /* Transformation Matrices: */
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  /* Game loop: */
  while (!glfwWindowShouldClose(window)) {

    // timing & input:
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float time = (float)glfwGetTime();
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int timeLoc = glGetUniformLocation(shader.ID, "iGlobalTime");
    glUniform1f(timeLoc, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);

    shader.use();

    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(camera.Zoom),
                         float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
    view = camera.GetViewMatrix();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    for (int i = 0; i < 10; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      angle += 1.0f;
      angle *= glfwGetTime();
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup:
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
  glfwTerminate();
  return 0;
}

unsigned int load_texture(const char *file, const unsigned short jpg) {

  // flip image:
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cerr << "Failed to load texture" << std::endl;
    return 1;
  }
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (jpg == 1) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
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
