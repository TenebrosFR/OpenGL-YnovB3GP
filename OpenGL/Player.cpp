#include "Player.h"

//Destructeur et constructeur
Player::~Player() {}
Player::Player(GLuint programID, VEC3 _position, float _height,float sensi) : position(_position),sensivity(sensi),Height(_height){
	cam = new Camera(programID, position, _height);
}
//Player Loop
void Player::newLoop(GLFWwindow* window, VECTOR<Mesh*> meshes) {
	//Time
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	//
	cam->newLoop(window, sensivity);
	//
	currentDirection = VEC3(0);
	listenInputs(window);
	Fall();
	checkIfIsOnMesh(meshes);
	Move();
	//
	cam->updateView(position);
	//Time
	lastTime = glfwGetTime();
}
void Player::checkIfIsOnMesh(VECTOR<Mesh*> meshes) {
	// Check collision with meshes to stop falling
	isGrounded = false;
	for (Mesh* mesh : meshes) {
		if (mesh->checkCollision(position - VEC3(0, Height / 2., 0),Height, Width)) {
			isGrounded = true;
			isJumping = isJumping;
			break;
		}
	}
}
void Player::listenInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_Z)) currentDirection.z += 1;
	if (glfwGetKey(window, GLFW_KEY_S)) currentDirection.z -= 1;
	if (glfwGetKey(window, GLFW_KEY_Q)) currentDirection.x += 1;
	if (glfwGetKey(window, GLFW_KEY_D)) currentDirection.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_SPACE) && isGrounded) Jump();
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
void Player::Fall() {
	//Si je suit et sol et que je ne saute pas OU que je suis au sol que j'ai sauter et que ce n'est pas sur cette frame
	if (isGrounded && !isJumping || isGrounded && isJumping && jumpTime != currentTime) {
		//J'arrete de sauter et je m'immobilise
		currentForce = 0;
		isJumping = false;
	} else { //En plein saut ou en pleine chute
		currentForce -= gravity * deltaTime;
		position.y += currentForce * deltaTime + 0.5f * gravity * deltaTime * deltaTime;
		cam->setPosition(position);
	}
}
void Player::Jump() {
	isJumping = true;
	jumpTime = currentTime;
	currentForce = jumpForce;
}
void Player::Crouch() {
	if (isCrouched) return;
	isCrouched = true;
	Height = Height / 2;
	position.y -= Height;
	cam->setPosition(position);
	cam->setHeight(Height);
}
void Player::Stand() {
	isCrouched = false;
	position.y += Height;
	Height = Height * 2;
	cam->setPosition(position);
	cam->setHeight(Height);
}