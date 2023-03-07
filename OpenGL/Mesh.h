#pragma once
#include "utils.h"
#include "objloader.hpp"
#include "vboindexer.hpp"
//
class Mesh
{
private:
	//ID
	GLuint elementBuffer, modelID, lightPosID, isTextureID, MatrixID, vertexArrayID, vertexBuffer, colorBuffer, uvBuffer, normalBuffer, texture, textureID;
	//VBO arrays
	GLfloat* vertices, uvs;
	//VBO .obk
	std::vector<glm::vec3> colors,loadVertices, loadNormals,verticesIndex, normalsIndex;;
	std::vector<glm::vec2> loadUV, uvsIndex;
	std::vector<unsigned short> indices;

	//storage
	glm::mat4 MVP, model;
	glm::vec3 position, size;
	int isTexture = 0;
public:
	//Mesh using .obj
	Mesh(GLuint programID, glm::vec3 _position, glm::vec3 _size, const char* objPath, const char* texturePath, std::vector<glm::vec3> _colors = std::vector<glm::vec3>());
	//Mesh using array
	Mesh(GLuint programID, glm::vec3 _position, glm::vec3 _size, std::vector<glm::vec3> _vertices, int verticeSize, std::vector<glm::vec2> _uvs, const char* texturePath, std::vector<glm::vec3> _colors = std::vector<glm::vec3>());
	~Mesh();


	void VAO(GLuint programID);
	void texturing(GLuint programID, const char* texturePath);
	void AddColor();
	void VBO();
	void CreateArrayVBOVec2(GLuint* buffer,int size, glm::vec2* adress);
	void CreateArrayVBOVec3(GLuint* buffer, int size, glm::vec3* adress);
	void CalculateMVP(glm::mat4 projection, glm::mat4 view);
	void Draw(glm::vec3 camPos);
	int MakeTampon(int attribArray, GLuint buffer, int vectorSize = 3);
};