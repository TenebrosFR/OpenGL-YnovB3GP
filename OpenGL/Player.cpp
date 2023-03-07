#include "Player.h"
#include <cmath>
//Destructeur et constructeur
Player::~Player() {}
Player::Player(GLuint programID, VEC3 _position, int _height,float sensi) : position(_position),sensivity(sensi),Height(_height){
	cam = new Camera(programID, position, _height);
}
//Player Loop
void Player::newLoop(GLFWwindow* window) {
	//Time
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	//
	cam->newLoop(window, sensivity);
	//
	currentDirection = VEC3(0);
	listenInputs(window);
	Move();
	//
	cam->updateView(position);
	//Time
	lastTime = glfwGetTime();
}
void Player::listenInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_Z)) currentDirection.z += 1;
	if (glfwGetKey(window, GLFW_KEY_S)) currentDirection.z -= 1;
	if (glfwGetKey(window, GLFW_KEY_Q)) currentDirection.x += 1;
	if (glfwGetKey(window, GLFW_KEY_D)) currentDirection.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_SPACE) || isJumping) Jump();
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) )	Crouch();
	else if (isCrouched) Stand();

	float yaw = glm::radians(cam->horizontalAngle);
	currentDirection = glm::rotateY(currentDirection, yaw);
}
//Actions
void Player::Move() {
	if ( (currentDirection == -lastDir && currentDirection != VEC3(0)) || lastDir == VEC3(0)) currentSpeed = 0.f;
	if (currentSpeed < moveSpeed) currentSpeed = std::lerp(currentSpeed, moveSpeed, accelerationTime);
	lastDir = currentDirection;
	if (currentDirection != VEC3(0)) position += NORMALIZE(currentDirection) * currentSpeed * deltaTime;
}

void Player::Jump() {
	if (!isJumping) {
		isJumping = true;
		currentForce = jumpForce;
	}else    currentForce -= gravity * deltaTime;
	if (isJumping) {
		position.y += currentForce * deltaTime + 0.5f * gravity * deltaTime * deltaTime;
		if (position.y <= 0) {
			position.y = 0;
			isJumping = false;
			currentForce = 0;
		}
		cam->setPosition(position);
	}
}
void Player::Crouch() {
	if (isCrouched) return;
	isCrouched = true;
	Height = Height / 2;
	cam->setHeight(Height);
}
void Player::Stand() {
	isCrouched = false;
	Height = Height * 2;
	cam->setHeight(Height);
}