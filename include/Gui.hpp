#pragma once

#include "RenderContext.hpp"
#include "imgui.h"
#include <imfilebrowser.h>
#include <GLFW/glfw3.h>

class Gui {
public:
  Gui(GLFWwindow *window, RenderContext &context, const char *glsl_version);
  static void NewFrame();
  void Update();
  void Render();
  void ShowCursor();
  void HideCursor();
  void setContext(RenderContext &context);

  void AppTab();
  void DirLightTab();
  void ModelTab(Model &model);
  ImGuiIO *GetIO();
  ~Gui();

private:
  ImGuiIO *io;
  GLFWwindow *m_window;
  ImGui::FileBrowser fileDialog;
  RenderContext &context;
};
