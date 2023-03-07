﻿#include "Player.h"
#include "Mesh.h"
#include "includes/shader.hpp"
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/glm-master/glm/ext/matrix_clip_space.hpp"
//Variables global
Player* player;
VECTOR<Mesh*> meshes;
double lastColorChangeTime = 0.0;
GLFWwindow* window;

GLfloat colorData[] = {
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,
};

void ViewProjection() {
	glm::mat4 view = player->getCamera()->getView();
	glm::mat4 projection = glm::perspective(90.f, 2560.f / 1440.f, 0.1f, 100.0f);
	glUniform3fv(*(player->viewPosID()), 1, &(player->getCamera()->getPos())[0]);

	for (Mesh* mesh : meshes)	mesh->CalculateMVP(projection, view);
}
void changeColor() {
	// get the current time
	double currentTime = glfwGetTime();
	if (currentTime - lastColorChangeTime > 4.0) {
		// update the time of the last color change
		lastColorChangeTime = currentTime;
		// generate random red, green, and blue values between 0 and 1
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		//
		colorData[0] = b;
		colorData[1] = g;
		colorData[2] = r;
		glClearColor(r, g, b, 1.0f);
	}
}

int myGLFW(void) {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(2560, 1440, "Ratio", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an intel gpu , they are not 3.3 compatible. Try the 2.1 version. /n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}


	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Seulement au moment de l'initialisation.
	GLuint programID = LoadShaders("VertexShader.vert", "FragmentShader.frag");
	player = new Player(programID,VEC3(0),1.8f);
	// generate random red, green, and blue values between 0 and 1
	float r = (float)rand() / RAND_MAX;
	float g = (float)rand() / RAND_MAX;
	float b = (float)rand() / RAND_MAX;
	colorData[0] = b;
	colorData[1] = g;
	colorData[2] = r;
	glClearColor(r, g, b, 1.0f);
	//
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	meshes.push_back(new Mesh(programID, VEC3(1, 2, 1), VEC3(1, 1, 1), "cube.obj","container.jpg"));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	do {
		//Clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//shader used
		glUseProgram(programID);
		//updates colors
		changeColor();
		//load rendering
		player->newLoop(window);
		ViewProjection();
		//Drawing
		for (Mesh* mesh : meshes)	mesh->Draw(player->getCamera()->getPos());
		//Memory
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	glfwWindowShouldClose(window);
	return 0;
}
int main(void) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	int errCode = myGLFW();
		return errCode;
}
