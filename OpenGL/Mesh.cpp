#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/stb-master/stb_image.h"

Mesh::Mesh(GLuint programID, VEC3 _position, VEC3 _size, VECTOR<VEC3> _vertices, int verticeSize, VECTOR<VEC2>  _uvs, const char* texturePath, VECTOR<VEC3> _colors) : position(_position), size(_size), colors(_colors)
{
	for (int i = 0; i < verticeSize; i += 3) {
		// Cr�ation des faces
		VEC3 v1 = VEC3(_vertices[i + 1].x - _vertices[i].x, _vertices[i + 1].y - _vertices[i].y, _vertices[i + 1].z - _vertices[i].z);
		VEC3 v2 = VEC3(_vertices[i + 2].x - _vertices[i].x, _vertices[i + 2].y - _vertices[i].y, _vertices[i + 2].z - _vertices[i].z);
		// Calcul de la normale du triangle
		VEC3 normal = glm::normalize(glm::cross(v1, v2));
		// On le met 3 fois pour le nombre de points
		for (int z = 0; z < 3; z++)loadNormals.push_back(normal);
	}
	indexVBO(_vertices, _uvs, loadNormals, indices, verticesIndex, uvsIndex, normalsIndex);
	VAO(programID);
	texturing(programID, texturePath);
	VBO();
	model = TRANSLATE(MATRIX(1.0f), position);
}

Mesh::Mesh(GLuint programID,VEC3 _position, VEC3 _size, const char* objPath, const char* texturePath, VECTOR<VEC3> _colors) : position(_position), size(_size), colors(_colors)
{
	loadOBJ(objPath, loadVertices, loadUV, loadNormals);
	indexVBO(loadVertices, loadUV, loadNormals, indices, verticesIndex, uvsIndex, normalsIndex);
	VAO(programID);
	texturing(programID, texturePath);
	VBO();
	model = TRANSLATE(MATRIX(1.0f), position);
}

void Mesh::VAO(GLuint programID)
{
	MatrixID = glGetUniformLocation(programID, "MVP");
	modelID = glGetUniformLocation(programID, "model");
	isTextureID = glGetUniformLocation(programID, "isTexture");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	viewPosID = glGetUniformLocation(programID, "viewPos");
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
	model = glm::scale(model, size);
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
	glUniform3fv(viewPosID, 1, &camPos[0]);
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