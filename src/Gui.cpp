#include "LightSourceDrawStrategy.hpp"
#include "imgui.h"
#include <Gui.hpp>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <model.hpp>
#include <iostream>
#include <ostream>

std::string trimPath(const std::string &globalPath,
                     const std::string &currentDir) {
  size_t pos = globalPath.find(currentDir);
  if (pos != std::string::npos) {
    pos += currentDir.length();
    if (globalPath[pos] == '/' || globalPath[pos] == '\\') {
      pos++;
    }
    return globalPath.substr(pos);
  }
  return globalPath; // If currentDir not found, return the original path
}

Gui::Gui(GLFWwindow *window, RenderContext &context, const char *glsl_version)
    : context(context) {
  // imgui init
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  this->io = &io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  this->m_window = window;
  fileDialog = ImGui::FileBrowser();
  fileDialog.SetTitle("Select a model");
  // ImGui::FileBrowser fileDialog;
}

void Gui::setContext(RenderContext &context) { this->context = context; }

Gui::~Gui() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Gui::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Gui::Render() {
  if (fileDialog.HasSelected()) {
    std::string modelPath = fileDialog.GetSelected().string();
    // TODO : GLOBAL APPNAME
    modelPath = trimPath(modelPath, "GLStuff");
    context.addModelByName(modelPath);
    fileDialog.ClearSelected();
  }
  fileDialog.Display();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::Update() {
  ImGui::Begin("Hello World");
  ImGui::Text("This is some useful text.");
  ImGui::End();
}

ImGuiIO *Gui::GetIO() { return this->io; }

void Gui::ShowCursor() { ImGui::GetIO().MouseDrawCursor = true; }
void Gui::HideCursor() { ImGui::GetIO().MouseDrawCursor = false; }

void Gui::AppTab() {

  // ImGui::ShowDemoWindow();
  if (ImGui::BeginTabItem("APP")) {
    glm::vec3 bg = context.getBgColor();
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Position: %.2f, %.2f, %.2f", context.getCamera().Position.x,
                context.getCamera().Position.y, context.getCamera().Position.z);
    if (ImGui::Button("Exit App")) {
      glfwSetWindowShouldClose(m_window, true);
    }
    if (ImGui::Button("Close Menu")) {
      context.showGui = false;
    }
    ImGui::Text("Background Color:");
    ImGui::ColorEdit3("##color", (float *)&bg);
    ImGui::EndTabItem();
    //
    context.setBgColor(bg);

    if (ImGui::Button("Import a model")) {
      fileDialog.Open();
    }
  }
}

void Gui::DirLightTab() {
  if (ImGui::BeginTabItem("DirLight")) {
    bool dirLight = context.getDirLightStatus();
    s_DirLightSettings *dirLightSettings = context.getDirLight();
    ImGui::Checkbox("Directional Light", &dirLight);
    ImGui::SliderFloat3("DirLight Direction",
                        (float *)&context.getDirLight()->direction, -10.0f,
                        10.0f);
    ImGui::ColorEdit3("Ambient", (float *)&dirLightSettings->ambient);
    ImGui::ColorEdit3("Diffuse", (float *)&dirLightSettings->diffuse);
    ImGui::ColorEdit3("Specular", (float *)&dirLightSettings->specular);
    ImGui::EndTabItem();
    //
    context.setDirLightStatus(dirLight);
  }
}

void Gui::ModelTab(Model &model) {
  if (ImGui::BeginTabItem(model.getName().c_str())) {

    // name:
    // char buf[32];
    // if (ImGui::InputText(model.getName().c_str(), buf, 32)) {
    //   model.setName(std::string(buf));
    // }

    // scale:
    float modelScale = model.getCurrentScale().x;
    if (ImGui::SliderFloat("Scale", (float *)&modelScale, 0.0f, 5.0f)) {
      glm::vec3 _scale = glm::vec3(modelScale);
      model.setScale(_scale);
    }

    // translation:
    glm::vec3 modelTranslation = model.getCurrentTranslation();
    if (ImGui::SliderFloat3("Translation", (float *)&modelTranslation, -20.0f,
                            20.0f)) {
      model.setTranslation(modelTranslation);
    }

    // rotation:
    static glm::vec3 modelRotation;
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(modelRotation), -180.0f,
                            180.0f)) {
      model.resetModelMatrix();

      model.rotate(modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
      model.rotate(modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      model.rotate(modelRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    if (ImGui::Button("Reset")) {
      model.resetModelMatrix();
    }

    static s_LightSettings lightSettings;
    if (ImGui::Button("Light Source")) {
      ImGui::ColorEdit3("Ambient", (float *)&lightSettings.ambient);
      ImGui::ColorEdit3("Diffuse", (float *)&lightSettings.diffuse);
      ImGui::ColorEdit3("Specular", (float *)&lightSettings.specular);
      ImGui::SliderFloat("Constant", &lightSettings.constant, 0.0f, 1.0f);
      ImGui::SliderFloat("Linear", &lightSettings.linear, 0.0f, 1.0f);
      ImGui::SliderFloat("Quadratic", &lightSettings.quadratic, 0.0f, 1.0f);
      lightSettings.position = model.getModelMatrix()[3];

      // check if model is already a lightsource
      if (dynamic_cast<LightSourceDrawStrategy *>(model.getDrawStrategy()) ==
          nullptr) {
        model.setAsLightSource(lightSettings);
      } else {
        dynamic_cast<LightSourceDrawStrategy *>(model.getDrawStrategy())
            ->setLightSettings(lightSettings);
      }
    } else {
      // if model is a lightsource:
      if (dynamic_cast<LightSourceDrawStrategy *>(model.getDrawStrategy()) !=
          nullptr) {
        model.unsetLightSource(); // so we don't "new DefaultDrawStrateg" every
                                  // frame
      }
    }
    ImGui::EndTabItem();
  }
}
