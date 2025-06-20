#include "Camera.h"

void Camera::update(const Window &window, const float &deltaTime) {
	updateVectors();
	updatePosition(window, deltaTime);
	updateDirection(window, deltaTime);
	updateZoom(deltaTime);

	m_viewMatrix = glm::lookAt(
		m_position,
		m_position + m_front,
		m_up
	);
}

glm::mat4 Camera::getViewMatrix() const {
	return m_viewMatrix;
}

float Camera::getFov() const {
	return m_fov;
}

void Camera::updateVectors() {
	glm::vec3 front = {
		std::cos(glm::radians(m_yaw)) *std::cos(glm::radians(m_pitch)),
			std::sin(glm::radians(m_pitch)),
			std::sin(glm::radians(m_yaw)) *std::cos(glm::radians(m_pitch))
	};

	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_up));
}

void Camera::updatePosition(const Window &window, const float &deltaTime) {
	float correctedSpeed = m_speed;
	if (InputHandler::isKeyDown(Key::LEFT_SHIFT)) {
		correctedSpeed *= 2.0f;
	} else if (InputHandler::isKeyDown(Key::LEFT_ALT)) {
		correctedSpeed *= 0.25f;
	}

	if (InputHandler::isKeyDown(Key::W)) m_position += m_front * correctedSpeed * deltaTime;
	if (InputHandler::isKeyDown(Key::A)) m_position -= m_right * correctedSpeed * deltaTime;
	if (InputHandler::isKeyDown(Key::S)) m_position -= m_front * correctedSpeed * deltaTime;
	if (InputHandler::isKeyDown(Key::D)) m_position += m_right * correctedSpeed * deltaTime;

	if (InputHandler::isKeyDown(Key::Q)) m_position.y -= correctedSpeed * deltaTime;
	if (InputHandler::isKeyDown(Key::E)) m_position.y += correctedSpeed * deltaTime;
}

void Camera::updateDirection(const Window &window, const float &deltaTime) {
	double xpos, ypos;
	glfwGetCursorPos(window.getWindow(), &xpos, &ypos);

	if (m_firstMouse) {
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset = xpos - m_lastX;
	float yoffset = m_lastY - ypos;
	m_lastX = xpos;
	m_lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f) {
		m_pitch = 89.0f;
	} else if (m_pitch < -89.0f) {
		m_pitch = -89.0f;
	}
}

void Camera::updateZoom(const float &deltaTime) {
	if (InputHandler::isKeyDown(Key::Z)) m_fov = glm::clamp(m_fov - m_zoomSpeed * deltaTime, 1.0f, m_maxFov);
	if (InputHandler::isKeyDown(Key::X)) m_fov = glm::clamp(m_fov + m_zoomSpeed * deltaTime, 1.0f, m_maxFov);
}