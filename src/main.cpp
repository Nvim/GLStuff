#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <alloca.h>
#include <iostream>
#include "shader.hpp"

// gl_Position's value determines where to place the vertex
const char *vertexShader = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "layout (location = 1) in vec3 aColor;\n"
                           "out vec3 ourColor;\n"
                           "void main()\n"
                           "{\n"
                           "gl_Position = vec4(aPos, 1.0);\n"
                           "ourColor = aColor;\n"
                           "}\0";

// FragColor = color displayed
const char *fragmentShader = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "in vec3 ourColor;\n"
                             //"uniform vec4 ourColor;\n"
                             "void main()\n"
                             "{\n"
                             "FragColor = vec4(ourColor, 1.0);\n"
                             "}\n\0";

// callback function for when we resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// returns a shader object
static unsigned int CompileShader(unsigned int type, const char *source) {
  unsigned int id = glCreateShader(type);
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);

  // error handling here
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message =
        (char *)alloca(length * sizeof(char)); // malloc but for the stack
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile shader:" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }
  return id;
}

// returns a shader program object
static unsigned int CreateShader() {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

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
  float vertices[] = {
      // positions        // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };
  unsigned int indicies[] = {0, 1, 2}; // so we dont hold duplicates
  unsigned int VBO, VAO, EBO;

  // Buffer Objects:
  glGenVertexArrays(1, &VAO); // gen VAO 1st
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  // Binding buffers:
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies,
               GL_STATIC_DRAW);

  // Set vertex attributes:
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)0); // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float))); // color attribute
  glEnableVertexAttribArray(1);

  // Shader Program:
  // unsigned int shader = CreateShader();
  Shader shader = Shader("res/shader.vert", "res/shader.frag");

  // game loop:
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT);
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float timeValue = glfwGetTime();
    float offset = sin(timeValue) / 2;
    // int vertexColorLocation = glGetUniformLocation(shader, "ourColor");
    shader.use();
    shader.setFloat("offset", offset);
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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
