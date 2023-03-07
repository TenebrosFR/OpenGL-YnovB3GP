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
	VECTOR<VEC3> colors,loadVertices, loadNormals,verticesIndex, normalsIndex;;
	VECTOR<VEC2> loadUV, uvsIndex;
	VECTOR<unsigned short> indices;

	//storage
	MATRIX MVP, model;
	VEC3 position, size;
	int isTexture = 0;
public:
	//Mesh using .obj
	Mesh(GLuint programID, VEC3 _position, VEC3 _size, const char* objPath, const char* texturePath, VECTOR<VEC3> _colors = VECTOR<VEC3>());
	~Mesh();


	void VAO(GLuint programID);
	void texturing(GLuint programID, const char* texturePath);
	void AddColor();
	void VBO();
	void CreateArrayVBOVec2(GLuint* buffer,int size, VEC2* adress);
	void CreateArrayVBOVec3(GLuint* buffer, int size, VEC3* adress);
	void CalculateMVP(MATRIX projection, MATRIX view);
	void Draw(VEC3 camPos);
	int MakeTampon(int attribArray, GLuint buffer, int vectorSize = 3);
	bool checkCollision(VEC3 position, float height, float width);
	bool intersectRayTriangle(VEC3 rayOrigin, VEC3 rayDirection, VEC3 v1, VEC3 v2, VEC3 v3);
};