#include "Camera.h"
#include "Mesh.h"
#include "stdio.h"
#include "stdlib.h"
#include "includes/shader.hpp"
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL/external/glm-master/glm/ext/matrix_clip_space.hpp"
#include <iostream>
#include <vector>


//Variables global
Camera* cam = new Camera(glm::vec3(0));
std::vector<Mesh*> meshes;
float size = 1.0f;
double lastColorChangeTime = 0.0;
GLuint MatrixID;
GLuint colorBuffer;
GLFWwindow* window;
GLuint cubeUV;
GLuint texture;
GLuint textureID;
float MoveSpeed = 0.1f;

// Un cube possde six faces avec deux triangles pour chaque, donc cela fait 6*2=12 triangles et 12*3=36 sommets


GLfloat cube_vertex[] = {
-1.0f,-1.0f,-1.0f, // triangle 1 : d�but
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : fin 
	1.0f, 1.0f,-1.0f, // triangle 2 : d�but 
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : fin 
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

GLfloat colorData[] = {
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,
};
GLfloat cube_color[] = {
0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};
GLfloat cube_uv[] = {
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.336024f, 1.0f - 0.671877f,
	0.667969f, 1.0f - 0.671889f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	0.336098f, 1.0f - 0.000071f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.336024f, 1.0f - 0.671877f,
	1.000004f, 1.0f - 0.671847f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.668104f, 1.0f - 0.000013f,
	0.336098f, 1.0f - 0.000071f,
	0.000103f, 1.0f - 0.336048f,
	0.000004f, 1.0f - 0.671870f,
	0.336024f, 1.0f - 0.671877f,
	0.000103f, 1.0f - 0.336048f,
	0.336024f, 1.0f - 0.671877f,
	0.335973f, 1.0f - 0.335903f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.667979f, 1.0f - 0.335851f
};

std::vector<glm::vec2> ToVec2(const GLfloat myArray[], int givenSize) {
	std::vector<glm::vec2> result;
	for (int i = 0; i < givenSize; i += 2) {
		glm::vec2 vec(myArray[i], myArray[i + 1]);
		result.push_back(vec);
	}
	return result;
}

std::vector<glm::vec3> ToVec3(const GLfloat myArray[],int givenSize) {
	std::vector<glm::vec3> result;
	for (int i = 0; i < givenSize; i += 3) {
		glm::vec3 vec(myArray[i], myArray[i + 1],myArray[i + 2]);
		result.push_back(vec);
	}
	std::vector<glm::vec3> one = result;
	return result;
}

void ViewProjection() {
	glm::mat4 view = cam->getView();
	glm::mat4 projection = glm::perspective(90.f, 2560.f / 1440.f, 0.1f, 100.0f);

	for (Mesh* mesh : meshes)	mesh->CalculateMVP(projection, view);
}
void UpdateCube() {
	// generate random red, green, and blue values between 0 and 1
	for (int i = 0; i < sizeof(cube_color) / 4; i += 3) {
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		cube_color[i] = r;
		cube_color[i + 1] = g;
		cube_color[i + 2] = b;
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
	}
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
		UpdateCube();
	}
}
double lastX = 0, lastY = 0;


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	size += yoffset * 0.1;
	if (size < 0.1) size = 0.1;
	if (size > 10) size = 10;
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

	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_cube), g_uv_buffer_data_cube, GL_STATIC_DRAW);
	// generate random red, green, and blue values between 0 and 1
	float r = (float)rand() / RAND_MAX;
	float g = (float)rand() / RAND_MAX;
	float b = (float)rand() / RAND_MAX;
	colorData[0] = b;
	colorData[1] = g;
	colorData[2] = r;
	glClearColor(r, g, b, 1.0f);
	cam->updateCamera(window);
	//
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	int verticeSize = sizeof(cube_vertex) / sizeof(cube_vertex[0]), colorSize = sizeof(cube_color) / sizeof(cube_color[0]), uvSize = sizeof(cube_uv) / sizeof(cube_uv[0]);
	meshes.push_back(new Mesh(programID, glm::vec3(1, 2, 1), glm::vec3(1, 1, 1), "cube.obj", ToVec3(cube_color,colorSize),"container.jpg"));
	meshes.push_back(new Mesh(programID, glm::vec3(3, 1, 3), glm::vec3(1, 1, 1), ToVec3(cube_vertex, verticeSize), verticeSize / 3, ToVec3(cube_color,colorSize), ToVec2(cube_uv, uvSize), "container.jpg"));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	do {
		//Clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//listen input
		cam->updateCamera(window);

		//shader used
		glUseProgram(programID);
		//updates colors
		changeColor();
		//load rendering
		ViewProjection();
		//Drawing
		for (Mesh* mesh : meshes)	mesh->Draw(cam->getPos());
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
