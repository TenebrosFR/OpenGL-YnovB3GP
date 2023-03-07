#include "Camera.h"

Camera::~Camera(){}

Camera::Camera(GLuint programID,VEC3 position,int height) : playerHeight(height){
	viewPos = VEC3(position.x,position.y + height,position.z);
	viewPosID = glGetUniformLocation(programID, "viewPos");
	view = MATRIX(1.0f);
}

void Camera::newLoop(GLFWwindow* window, float sensivity) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	//Nouvel angle de vue horizontal
	horizontalAngle += sensivity * float(lastX - xpos);
	//Nouvel angle de vue vertical que je limite pour ne pas faire des salto avec la caméra
	verticalAngle = glm::clamp(verticalAngle + sensivity * float(lastY - ypos), -89.0f, 89.0f);
	lastX = xpos, lastY = ypos;
}
void Camera::updateView( VEC3 position) {
	viewPos = VEC3(position.x, position.y + playerHeight, position.z);
	viewDirection = VEC3(
		cos(glm::radians(verticalAngle)) * sin(glm::radians(horizontalAngle)),
		sin(glm::radians(verticalAngle)),
		cos(glm::radians(verticalAngle)) * cos(glm::radians(horizontalAngle))
	);
	VEC3 up(0, 1, 0);
	view = glm::lookAt(viewPos, viewPos + viewDirection, up);
}

