#include <stdio.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <imgui_internal.h>
#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <InputManager/InputManager.hpp>
#include <UI/Common/Style.hpp>
#include <UI/UI.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/FilesystemWidget.hpp>
#include <UI/Widgets/InspectorWidget.hpp>
#include <UI/Widgets/ObjectsWidget.hpp>
#include <UI/Widgets/ProfilerWidget.hpp>
#include <UI/Widgets/PropertiesWidget.hpp>
#include <UI/Widgets/RuntimeWidget.hpp>
#include <UI/Widgets/SceneWidget.hpp>

namespace shkyera {

UI::UI() : _open(true) {
  initializeImgui();
  styleImgui();
}

void UI::initialize(std::shared_ptr<Registry> registry) {
  _registry = std::move(registry);

  initializeWidgets();
}

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void UI::initializeImgui() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);             // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // // 3.2+ only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // // 3.0+ only
#endif

  // Create window with graphics context
  _window = glfwCreateWindow(1400, 750, "Shkyera Engine", NULL, NULL);
  if (_window == NULL)
    return;

  glfwMakeContextCurrent(_window);
  glfwSwapInterval(1);  // Enable vsync
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void UI::initializeWidgets() {
  _widgets.emplace_back(std::make_unique<ConsoleWidget>("Console"));
  _widgets.emplace_back(std::make_unique<PropertiesWidget>(_registry));
  _widgets.emplace_back(std::make_unique<InspectorWidget>("Inspector", _registry));
  _widgets.emplace_back(std::make_unique<CameraPropertiesWidget>(_registry));
  _widgets.emplace_back(std::make_unique<EnvironmentPropertiesWidget>(_registry));
  _widgets.emplace_back(std::make_unique<ProfilerWidget>("Profiler"));

  _widgets.emplace_back(std::make_unique<SceneWidget>(_registry));
  _widgets.emplace_back(std::make_unique<RuntimeWidget>(_registry));

  auto objectsWidget = std::make_unique<ObjectsWidget>("Objects");
  objectsWidget->setRegistry(_registry);
  _widgets.emplace_back(std::move(objectsWidget));

  _widgets.emplace_back(std::make_unique<FilesystemWidget>("Assets", _registry));
}

void UI::styleImgui() {
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowMinSize = ImVec2(150, 150);
  style.ScrollbarSize = 12.0f;
  style.ScrollbarRounding = 16.0f;

  style::loadFonts("resources/fonts/OpenSansRegular.ttf");
  using namespace shkyera::style;

  /********        TEXT       *********/
  style.Colors[ImGuiCol_Text] = TEXT_COLOR;
  style.Colors[ImGuiCol_TextDisabled] = DISABLED_TEXT_COLOR;

  /********        WINDOW     *********/
  style.Colors[ImGuiCol_WindowBg] = BACKGROUND_COLOR;
  style.Colors[ImGuiCol_BorderShadow] = BLACK;

  /********      SCROLLBAR    *********/
  style.Colors[ImGuiCol_ScrollbarBg] = ACCENT_COLOR;

  /********        FRAME      *********/
  style.Colors[ImGuiCol_FrameBg] = ACCENT_COLOR;
  style.Colors[ImGuiCol_FrameBgHovered] = ACCENT_COLOR;
  style.Colors[ImGuiCol_FrameBgActive] = ACCENT_COLOR;

  /********        TABS     *********/
  style.Colors[ImGuiCol_Tab] = ACCENT_COLOR;
  style.Colors[ImGuiCol_TabHovered] = STRONG_ACCENT_COLOR;
  style.Colors[ImGuiCol_TabUnfocused] = ACCENT_COLOR;
  style.Colors[ImGuiCol_TabUnfocusedActive] = STRONG_ACCENT_COLOR;
  style.Colors[ImGuiCol_TabActive] = STRONG_ACCENT_COLOR;

  /********        TITLE      *********/
  style.Colors[ImGuiCol_TitleBg] = BACKGROUND_COLOR;
  style.Colors[ImGuiCol_TitleBgActive] = BACKGROUND_COLOR;
  style.Colors[ImGuiCol_TitleBgCollapsed] = BACKGROUND_COLOR;

  /********      HEADERS     ********/
  style.Colors[ImGuiCol_Header] = ACCENT_COLOR;
  style.Colors[ImGuiCol_HeaderActive] = STRONG_ACCENT_COLOR;
  style.Colors[ImGuiCol_HeaderHovered] = STRONG_ACCENT_COLOR;

  /********       SLIDER      *********/
  style.Colors[ImGuiCol_SliderGrab] = STRONG_ACCENT_COLOR;
  style.Colors[ImGuiCol_SliderGrabActive] = STRONG_ACCENT_COLOR;

  /********      SEPARATOR     ********/
  style.Colors[ImGuiCol_Separator] = GREY;
  style.Colors[ImGuiCol_SeparatorHovered] = LIGHT_GREY;
  style.Colors[ImGuiCol_SeparatorActive] = LIGHT_GREY;

  /********       BUTTON      *********/
  style.Colors[ImGuiCol_Button] = ACCENT_COLOR;
  style.Colors[ImGuiCol_ButtonHovered] = STRONG_ACCENT_COLOR;
  style.Colors[ImGuiCol_ButtonActive] = STRONG_ACCENT_COLOR;

  /********     RESIZE GRIP   *********/
  style.Colors[ImGuiCol_ResizeGrip] = ACCENT_COLOR;
  style.Colors[ImGuiCol_ResizeGripHovered] = STRONG_ACCENT_COLOR;
  style.Colors[ImGuiCol_ResizeGripActive] = STRONG_ACCENT_COLOR;

  /********       DOCKING     *********/
  style.Colors[ImGuiCol_DockingPreview] = ACCENT_COLOR;
  style.Colors[ImGuiCol_DockingEmptyBg] = ACCENT_COLOR;
}

void UI::beginFrame() {
  SHKYERA_PROFILE("UI::beginFrame");

  glfwPollEvents();

  glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                 ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

  ImGui::Begin("Shkyera Engine", &_open, windowFlags);
  ImGui::PopStyleVar();

  ImGuiIO& io = ImGui::GetIO();

  ImGuiID dockspace_id = ImGui::GetID("Shkyera Engine");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                   ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);

  static auto firstTime = true;
  if (firstTime) {
    firstTime = false;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    ImGuiID dock_id_right, dock_id_left, dock_id_left_bottom, dock_id_left_up, dock_id_left_up_left,
        dock_id_left_up_right;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.75f, &dock_id_left, &dock_id_right);
    ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.3f, &dock_id_left_bottom, &dock_id_left_up);
    ImGui::DockBuilderSplitNode(dock_id_left_up, ImGuiDir_Left, 0.3f, &dock_id_left_up_left, &dock_id_left_up_right);

    ImGui::DockBuilderDockWindow("Objects", dock_id_left_up_left);
    ImGui::DockBuilderDockWindow("Scene", dock_id_left_up_right);
    ImGui::DockBuilderDockWindow("Runtime", dock_id_left_up_right);
    ImGui::DockBuilderDockWindow("Properties", dock_id_right);
    ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
    ImGui::DockBuilderDockWindow("Scene Camera", dock_id_right);
    ImGui::DockBuilderDockWindow("Profiler", dock_id_right);
    ImGui::DockBuilderDockWindow("Environment", dock_id_right);
    ImGui::DockBuilderDockWindow("Assets", dock_id_left_bottom);
    ImGui::DockBuilderDockWindow("Console", dock_id_left_bottom);

    ImGui::DockBuilderFinish(dockspace_id);
  }

  ImGui::End();
}

void UI::renderFrame() {
  SHKYERA_PROFILE("UI::renderFrame");

  const auto& windowSize = ImGui::GetWindowSize();
  InputManager::getInstance().setCoordinateSystem(InputManager::CoordinateSystem::ABSOLUTE, {0, 0},
                                                  {windowSize.x, windowSize.y});
  InputManager::getInstance().processInput(_window);

  for (const auto& w : _widgets) {
    w->draw();
  }

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open Project", "Ctrl+O")) {}
      if (ImGui::MenuItem("Save", "Ctrl+S")) {}
      if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window")) {
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  static auto onlyOnce = [this]() {
    ImGui::SetWindowFocus("Properties");
    return true;
  }();
}

void UI::endFrame() {
  {
    SHKYERA_PROFILE("UI::endFrame");
    ImGui::Render();

    int display_w, display_h;

    glfwGetFramebufferSize(_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1, 0.1, 0.1, 0.1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window);
  }
}

void UI::draw() {
  _open = !glfwWindowShouldClose(_window);
  if (_open) {
    beginFrame();
    renderFrame();
    endFrame();
  } else {
    close();
  }
}

void UI::close() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(_window);
  glfwTerminate();
}

bool UI::shouldClose() const {
  return !_open;
}

}  // namespace shkyera
