#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>
#include <iostream>

class Window {
public:
	Window(const std::uint32_t &width, const std::uint32_t &height, const std::string &name);
	~Window();

	bool shouldClose() const;
	void update();

	std::uint32_t getWidth() const;
	std::uint32_t getHeight() const;
	float getDeltaTime() const;
	GLFWwindow *getWindow() const;
private:
	static void glfwResizeCallback(GLFWwindow *window, int width, int height);
	void createGLFWWindow();
	void updateDeltaTime();
	void updateFPS();

	std::uint32_t m_width;
	std::uint16_t m_height;
	std::string m_name;
	GLFWwindow *m_window;

	float m_prevFrameTime;
	float m_deltaTime;

	float m_frameCount;
	float m_fps;
	float m_prevFPSUpdateTime;
};

#endif