#pragma once

#include "RenderContext.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>

class Gui {
public:
  Gui(GLFWwindow *window, const char *glsl_version);
  static void NewFrame();
  void Update();
  void Render();
  void ShowCursor();
  void HideCursor();

  void AppTab(RenderContext &context);
  void DirLightTab(RenderContext &context);
  void ModelTab(Model &model);
  ImGuiIO *GetIO();
  ~Gui();

private:
  ImGuiIO *io;
  GLFWwindow *m_window;
};
