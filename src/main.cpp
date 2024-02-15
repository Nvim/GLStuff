#include <algorithm>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <alloca.h>
#include <iostream>
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
#define glCheckError() glCheckError_(__FILE__, __LINE__)

// callback function for when we resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

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
      glfwCreateWindow(800, 600, "triangle qui bouge", NULL, NULL);
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
  glViewport(0, 0, 800, 600);

  // call our function when window is resized:
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /*
    RENDERING !
  */

  // Shader Program:
  // unsigned int shader = CreateShader();
  Shader shader = Shader("res/shader.vert", "res/fade.frag");

  // clang-format off
  float vertices[] = {
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
  // clang-format on

  // Buffer Objects:
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO); // gen VAO 1st
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  // Binding buffers:
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Set vertex attributes:
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)0); // position attribute
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load texture:
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("res/container.jpg", &width, &height, &nrChannels, 0);
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

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);

  // game loop:
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT);
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float time = glfwGetTime();
    int vertexColorLocation = glGetUniformLocation(shader.ID, "iGlobalTime");
    glUniform1f(vertexColorLocation, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup:
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();
  return 0;
}
