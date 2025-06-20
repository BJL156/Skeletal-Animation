#include "Window.h"

Window::Window(const std::uint32_t &width, const std::uint32_t &height, const std::string &name)
  : m_width(width), m_height(height), m_name(name) {
  createGLFWWindow();
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::update() {
  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window, true);
  }

  updateDeltaTime();
  updateFPS();

  glfwSwapBuffers(m_window);
  glfwPollEvents();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::uint32_t Window::getWidth() const {
    return m_width;
}

std::uint32_t Window::getHeight() const {
    return m_height;
}

float Window::getDeltaTime() const {
  return m_deltaTime;
}

GLFWwindow *Window::getWindow() const {
    return m_window;
}

void Window::glfwResizeCallback(GLFWwindow *window, int width, int height) {
  Window *windowHandle = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  windowHandle->m_width = width;
  windowHandle->m_height = height;

  glViewport(0, 0, width, height);
}

void Window::createGLFWWindow() {
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW.\n";
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
  if (m_window == nullptr) {
    std::cout << "Failed to create GLFW window.\n";
    return;
  }

  glfwMakeContextCurrent(m_window);

  glfwSwapInterval(0);

  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, glfwResizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD.\n";
    return;
  }

  glViewport(0, 0, m_width, m_height);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::updateDeltaTime() {
  float currentFrameTime = glfwGetTime();
  m_deltaTime = currentFrameTime - m_prevFrameTime;
  m_prevFrameTime = currentFrameTime;
}

void Window::updateFPS() {
  float currentFrameTime = static_cast<float>(glfwGetTime());
  const float elapsedTime = currentFrameTime - m_prevFPSUpdateTime;

  m_frameCount++;

  if (currentFrameTime - m_prevFPSUpdateTime >= 1.0f) {
    m_fps = m_frameCount / elapsedTime;
    float averageFrameTime = elapsedTime / m_frameCount;
    float milliseconds = averageFrameTime * 1000.0f;

    std::string newName = m_name + " | FPS: " + std::to_string(m_fps) + " MS: " + std::to_string(milliseconds);
    glfwSetWindowTitle(m_window, newName.c_str());

    m_frameCount = 0;
    m_prevFPSUpdateTime = currentFrameTime;
  }
}