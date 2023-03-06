#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <C:/Tout/Cours/3eme annee/OpenGl/OpenGL/external/glm-master/glm/glm.hpp>
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL/external/glm-master/glm/ext/matrix_transform.hpp"
#include <iostream>

class Camera
{
private:

	glm::mat4 view;
	float lastX, horizontalAngle, verticalAngle;
	glm::vec3 lastPos, m_position, m_direction, m_right, m_up;
	double lastTime, currentTime;
	float sensivity, speed, rotationSpeed;

public:

	Camera(glm::vec3 position,float sensi =  0.05f);

	~Camera();

	inline glm::vec3 getPos() { return m_position; }

	inline glm::vec3 Multiply(glm::vec3 in, int value) { return glm::vec3(in.x * value, in.y * value, in.z * value); }

	inline glm::mat4 getView() { return view; }

	void updateCamera(GLFWwindow* window);
	
	void updateView();

	void listenInputs(GLFWwindow* window);

	void camRotation(GLFWwindow* window, double xpos, double ypos);

	glm::vec3 getDirection() {
		return glm::vec3(
			cos(glm::radians(horizontalAngle)),
			0,
			sin(glm::radians(horizontalAngle))
		);
	}
	glm::vec3 getRight() {
		return glm::normalize(glm::cross(getDirection(), glm::vec3(0, 1, 0)));
	}
};