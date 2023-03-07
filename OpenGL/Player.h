#pragma once
#include "Camera.h"
#include <cmath>
#include "Mesh.h"
//
class Player
{
private:
	Camera* cam;
	VEC3 lastDir,currentDirection,position;
	float sensivity, rotationSpeed;
	float Height, Width = 0.5f;
	bool isCrouched,isJumping,isGrounded;
	float gravity = 9.81f * 3.f, jumpForce = 5.f * 2.f,currentForce = 0.f,  moveSpeed = 7.5f, currentSpeed = 0.f, accelerationTime = 2.f;
	float lastTime,deltaTime,currentTime,jumpTime;
public:
	Player(GLuint programID, VEC3 _position, float height, float sensi = 0.05f);
	~Player();

	//Camera getter
	inline Camera* getCamera() { return cam; }
	inline GLuint* viewPosID() { return &(cam->viewPosID); }
	//MainLoop for player
	void newLoop(GLFWwindow* window, VECTOR<Mesh*> meshes);
	void checkIfIsOnMesh(VECTOR<Mesh*> meshes);
	void listenInputs(GLFWwindow* window);
	//For Movements
	inline VEC3 getRight() {	return NORMALIZE(glm::cross(cam->getDirection(), VEC3(0, 1, 0)));	}
	inline VEC3 getPos() { return position; }
	inline VEC3 Multiply(VEC3 in, int value) { return VEC3(in.x * value, in.y * value, in.z * value); }
	//
	void Crouch(), Stand(), Jump(), Fall(), Move();
};