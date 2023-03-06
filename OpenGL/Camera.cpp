#include "Camera.h"

Camera::~Camera(){}

Camera::Camera(glm::vec3 position, float sensi) : horizontalAngle(0.0f) {
	m_position = position;
	view = glm::mat4(1.0f);
	sensivity = sensi;
}

void Camera::updateCamera(GLFWwindow* window) {
	listenInputs(window);
	updateView();
}

void Camera::updateView() {
	//view = glm::lookAt(m_position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 direction(
		cos(glm::radians(horizontalAngle)),
		0,
		sin(glm::radians(horizontalAngle))
	);
	glm::vec3 up(0, 1, 0);
	view = glm::lookAt(m_position, m_position + direction, up);
}

void Camera::listenInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_Z)) m_position += 0.1f * getDirection();;
	if (glfwGetKey(window, GLFW_KEY_S)) m_position -= 0.1f * getDirection();;
	if (glfwGetKey(window, GLFW_KEY_Q)) m_position -= 0.1f * getRight();
	if (glfwGetKey(window, GLFW_KEY_D)) m_position += 0.1f * getRight();

	if (glfwGetKey(window, GLFW_KEY_SPACE)) m_position += 0.1f * glm::vec3(0, 1, 0);;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) m_position -= 0.1f * glm::vec3(0, 1, 0);;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		horizontalAngle += sensivity * float(xpos - lastX);
		lastX = xpos;
	}
}
