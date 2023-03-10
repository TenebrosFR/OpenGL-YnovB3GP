#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/stb-master/stb_image.h"

Mesh::Mesh(GLuint programID,VEC3 _position, VEC3 _size, const char* objPath, const char* texturePath, VECTOR<VEC3> _colors) : position(_position), size(_size), colors(_colors)
{
	loadOBJ(objPath, loadVertices, loadUV, loadNormals);
	indexVBO(loadVertices, loadUV, loadNormals, indices, verticesIndex, uvsIndex, normalsIndex);
	VAO(programID);
	texturing(programID, texturePath);
	VBO();
	model = glm::scale(TRANSLATE(MATRIX(1.0f), position), size);
}
void Mesh::VAO(GLuint programID)
{
	MatrixID = glGetUniformLocation(programID, "MVP");
	modelID = glGetUniformLocation(programID, "model");
	isTextureID = glGetUniformLocation(programID, "isTexture");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	//Creation de mesh
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
}

void Mesh::texturing(GLuint programID,const char* texturePath)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	//cube texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		textureID = glGetUniformLocation(programID, "textureSampler");
		isTexture = 1;
	}else std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
}
void Mesh::AddColor() {
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size(), &colors[0], GL_STATIC_DRAW);
}
void Mesh::VBO()
{
	//indices
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	if (colors.size() != 0) AddColor();
	CreateArrayVBOVec3(&vertexBuffer, verticesIndex.size() * sizeof(VEC3), &verticesIndex[0]);
	CreateArrayVBOVec2(&uvBuffer, uvsIndex.size() * sizeof(VEC2), &uvsIndex[0]);
	CreateArrayVBOVec3(&normalBuffer, normalsIndex.size() * sizeof(VEC3), &normalsIndex[0]);
}

void Mesh::CreateArrayVBOVec3(GLuint* buffer, int size, VEC3* adress) {
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ARRAY_BUFFER, size, adress, GL_STATIC_DRAW);
}
void Mesh::CreateArrayVBOVec2(GLuint* buffer, int size, VEC2* adress) {
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ARRAY_BUFFER, size, adress, GL_STATIC_DRAW);
}

void Mesh::CalculateMVP(MATRIX projection, MATRIX view)
{ 
	MVP = projection * view * model;
}
int Mesh::MakeTampon(int attribArray, GLuint buffer,int vectorSize) {
	glEnableVertexAttribArray(attribArray);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(attribArray, vectorSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return attribArray;
}
void Mesh::Draw(VEC3 camPos)
{
	VECTOR<int> ints = VECTOR<int>();
	glUniform3fv(lightPosID, 1, &camPos[0]);
	glUniform1i(isTextureID, isTexture);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	ints.push_back(MakeTampon(0, vertexBuffer));
	if (colors.size() != 0) ints.push_back(MakeTampon(1, colorBuffer));
	ints.push_back(MakeTampon(2, uvBuffer, 2));
	ints.push_back(MakeTampon(3, normalBuffer));
	//texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);
	//draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawElements(
		GL_TRIANGLES, // mode
		indices.size(), // nombre
		GL_UNSIGNED_SHORT, // type
		(void*)0 // décalage du tableau de tampons
	);
	for (int element : ints) glDisableVertexAttribArray(element); //memory
}

//Collisions 
bool Mesh::checkCollision(VEC3 playerPosition, float height, float width) {
	// Calculate the maximum and minimum bounds of the mesh
	VEC3 meshMin = position - size ;
	VEC3 meshMax = position + size ;

	// Calculate the maximum and minimum bounds of the object
	VEC3 objectMin = playerPosition - VEC3(width, height, width);
	VEC3 objectMax = playerPosition + VEC3(width, height, width);


	// Check if the bounds overlap
	bool overlapX = (meshMin.x <= objectMax.x && meshMax.x >= objectMin.x);
	bool overlapY = (meshMin.y <= objectMax.y && meshMax.y >= objectMin.y);
	bool overlapZ = (meshMin.z <= objectMax.z && meshMax.z >= objectMin.z);

	return overlapX && overlapY && overlapZ;
}

bool Mesh::intersectRayTriangle(VEC3 rayOrigin, VEC3 rayDirection, VEC3 v1, VEC3 v2, VEC3 v3) {
	VEC3 edge1 = v2 - v1;
	VEC3 edge2 = v3 - v1;
	VEC3 h = cross(rayDirection, edge2);
	float a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON) {
		return false;
	}
	float f = 1 / a;
	VEC3 s = rayOrigin - v1;
	float u = f * dot(s, h);
	if (u < 0 || u > 1) {
		return false;
	}
	VEC3 q = cross(s, edge1);
	float v = f * dot(rayDirection, q);
	if (v < 0 || u + v > 1) {
		return false;
	}
	float t = f * dot(edge2, q);
	return (t > EPSILON);
}