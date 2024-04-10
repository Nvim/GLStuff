#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "DefaultDrawStrategy.hpp"
#include "Gui.hpp"
#include "Scene.hpp"
#include "s_Matrices.hpp"
#include "s_MouseInput.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <vao.hpp>
#include <vbo.hpp>
#include <ebo.hpp>
#include <mesh.hpp>
#include <texture.hpp>
#include "model.hpp"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// functions:
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

unsigned int load_texture(const char *file, const unsigned short jpg);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void cursor_enter_callback(GLFWwindow *window, int entered);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, RenderContext &context);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

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

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

  // call our function when window is resized:
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetCursorEnterCallback(window, cursor_enter_callback);
  glfwSetKeyCallback(window, key_callback);

  /*
    RENDERING !
  */

  /* Shader Program: */
  Shader litShader("res/Shaders/lighting/lit.vert",
                   "res/Shaders/lighting/litFinal.frag");
  Shader lightSourceShader("res/Shaders/lighting/lightSource.vert",
                           "res/Shaders/lighting/lightSource.frag");

  /* Models */
  // Model backpack("res/Models/Helmet/DamagedHelmet.gltf");
  // Model cube("res/cube/cube.obj");
  // backpack.loadModelVerbose();
  // cube.loadModel();

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

  s_DirLightSettings dirLightSettings;
  dirLightSettings.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
  dirLightSettings.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
  dirLightSettings.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
  dirLightSettings.specular = glm::vec3(1.0f, 1.0f, 1.0f);

  /* Camera: */
  Camera camera(glm::vec3(0.0f, 0.0f, 17.0f));
  s_MouseInput mouseInput;

  RenderContext context(litShader);
  context.setShader(litShader);
  context.setCamera(camera);
  context.setMouseInput(mouseInput);
  context.setMatrices(matrices);
  context.setBgColor(glm::vec3(0.43f, 0.59f, 0.82f));
  context.enableDirLight();
  context.setDirLightSettings(dirLightSettings);

  Gui gui(window, context, "#version 330");

  Model helmet("res/Models/Helmet/DamagedHelmet.gltf");
  Model backpack("res/Models/Backpack/backpack.obj");
  context.addModel(backpack);
  context.addModel(helmet);
  context.loadModels();

  DefaultDrawStrategy defaultStrat;
  for (Model *m : context.models) {
    m->setDrawStrategy(defaultStrat);
  }

  // backpack.setDrawStrategy(defaultStrat);
  // cube.setDrawStrategy(defaultStrat);
  // cube.translate(glm::vec3(0.0f, 0.0f, 0.0f));
  // cube.scale(glm::vec3(0.5f, 0.5f, 0.5f));

  // context.addLightSource(cube, defaultLightSettings);
  // defaultLightSettings.position = cube.getModelMatrix()[3];
  // cube.setAsLightSource(defaultLightSettings);

  glfwSetWindowUserPointer(window, &context);
  std::cout << "Entering game loop" << std::endl;

  unsigned int counter = 0;
  bool dirLight;
  float tmpScale = 1.0f;

  /* ***************************************************************** */
  /* Game loop: */
  /* ***************************************************************** */

  while (!glfwWindowShouldClose(window)) {
    // timing & input:
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // counter++;
    // if (deltaTime >= 1.0f / 30.0f) {
    //   std::string fps = std::to_string((1.0f / deltaTime) * counter);
    //   glfwSetWindowTitle(window, fps.c_str());
    //   counter = 0;
    //   lastFrame = currentFrame;
    //   counter = 0;
    // }
    glfwPollEvents();
    processInput(window, context);

    Gui::NewFrame();
    if (context.showGui) {
      gui.ShowCursor();
      ImGui::Begin("MENU");

      if (ImGui::BeginTabBar("MyTabBar")) {

        gui.AppTab();
        gui.DirLightTab();
        for (Model *m : context.models) {
          gui.ModelTab(*m);
        }
        //
        // if (ImGui::BeginTabItem("Model")) {
        //   glm::vec3 tmpPos = backpack.getModelMatrix()[3];
        //   ImGui::SliderFloat3("Position", (float *)&tmpPos, -20.0f, 20.0f);
        //   ImGui::SliderFloat("Scale: ", &tmpScale, 0.0f, 5.0f);
        //   backpack.translate(tmpPos);
        //   backpack.scale(glm::vec3(tmpScale));
        //   ImGui::EndTabItem();
        // }

        ImGui::EndTabBar();
      }
      ImGui::End();
    } else {
      gui.HideCursor();
    }

    glClearColor(context.getBgColor().x, context.getBgColor().y,
                 context.getBgColor().z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    matrices.projection =
        glm::perspective(glm::radians(context.getCamera().Zoom),
                         float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
    matrices.view = context.getCamera().GetViewMatrix();
    context.setMatrices(matrices);

    // float x = 1.0f + sin(time) * radius;
    // float y = sin(time / 2.0f) * 2.5f;
    // cube.resetModelMatrix();
    // cube.translate(glm::vec3(-6.0f, 0.0f, 7.0f));
    // cube.translate(glm::vec3(x, y, z));
    // defaultLightSettings.ambient.z = sin(time) * 2.0f;

    for (Model *m : context.models) {
      m->Draw(context);
    }
    // cube.Draw(context);

    gui.Render();
    glfwSwapBuffers(window);
  }

  // cleanup:
  // texHuh.Delete();
  litShader.Delete();
  lightSourceShader.Delete();
  // ImGui_ImplOpenGL3_Shutdown();
  // ImGui_ImplGlfw_Shutdown();
  // ImGui::DestroyContext();
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

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  RenderContext &context = *(RenderContext *)glfwGetWindowUserPointer(window);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    if (context.showGui) {
      context.showGui = false;
    } else {
      // center the cursor before showing gui:
      glfwSetCursorPos(window, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
      context.showGui = true;
    }
  }
}

void processInput(GLFWwindow *window, RenderContext &context) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    context.getCamera().ProcessKeyboard(RIGHT, deltaTime);
  // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  //   glfwSetCursorPosCallback(window, mouse_callback);
  // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  //   context.getCamera().ProcessArrows(DIR_UP, 1.0f);
  // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  //   context.getCamera().ProcessArrows(DIR_DOWN, 1.0f);
  // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  //   context.getCamera().ProcessArrows(DIR_LEFT, 1.0f);
  // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  //   context.getCamera().ProcessArrows(DIR_RIGHT, 1.0f);
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

  if (!context->showGui) {
    context->getCamera().ProcessMouseMovement(xOffset, yOffset);
  }
  context->setMouseInput(in);
}

void cursor_enter_callback(GLFWwindow *window, int entered) {
  if (entered) {
    glfwSetCursorPos(window, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  RenderContext *context = (RenderContext *)glfwGetWindowUserPointer(window);
  if (!context->showGui) {
    context->getCamera().ProcessMouseScroll(static_cast<float>(yoffset));
  }
}
