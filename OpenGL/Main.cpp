#include "Player.h"
#include "Mesh.h"
#include "Maping.h"
#include "includes/shader.hpp"
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/glm-master/glm/ext/matrix_clip_space.hpp"
//Variables global
Player* player;
VECTOR<Mesh*> meshes;
double lastColorChangeTime = 0.0;
GLFWwindow* window;
const GLFWvidmode* mode;
int screenWidth, screenHeight;
GLfloat colorData[] = {
	0.0f, 0.0f, 0.2f, // bleu foncé
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};

void ViewProjection() {
	glm::mat4 view = player->getCamera()->getView();
	float aspectRatio = (float)mode->width / (float)mode->height;
	float fovy = 2 * atan(tan(70.f / 2) * aspectRatio);
	glm::mat4 projection = glm::perspective(fovy, aspectRatio, 0.1f, 100.0f);
	glUniform3fv(*(player->viewPosID()), 1, &(player->getCamera()->getPos())[0]);

	for (Mesh* mesh : meshes)	mesh->CalculateMVP(projection, view);
}

int myGLFW(void) {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (!mode) return -1;
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Virgil's Plateformer", glfwGetPrimaryMonitor(), NULL);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.33f, 1.0f);
	//Fin de l'init des packages
	GLuint programID = LoadShaders("VertexShader.vert", "FragmentShader.frag");
	player = new Player(programID,VEC3(0, 6, 0),1.8f);
	meshes = meshesOfMap(programID);
	do {
		//Clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//shader used
		glUseProgram(programID);
		//load rendering
		player->newLoop(window,meshes);
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
