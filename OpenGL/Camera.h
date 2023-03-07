#pragma once
#include "utils.h"
//
class Camera
{
private:
	VEC3 lastPos, viewPos,viewDirection;
	MATRIX view;
public:
	float playerHeight,lastX = 0.f, lastY = 0.f, horizontalAngle = 0.f, verticalAngle = 0.f;
	GLuint viewPosID;

	Camera(GLuint programID,VEC3 position, int height);


	~Camera();

	//getter
	inline MATRIX getView() { return view; }
	inline VEC3 getPos() { return viewPos; }
	inline VEC3 getDirection() { return NORMALIZE(viewDirection); }

	void newLoop(GLFWwindow* window,float sensivity);
	void updateView(VEC3 position);
	void camRotation(GLFWwindow* window, double xpos, double ypos);

	//Setter
	inline void setHeight(float newHeight) { playerHeight = newHeight; }
	inline void setPosition(VEC3 position){	viewPos = VEC3(position.x, position.y + playerHeight, position.z); }
};