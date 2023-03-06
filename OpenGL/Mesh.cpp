#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL/external/stb-master/stb_image.h"

Mesh::Mesh(GLuint programID,glm::vec3 _position, glm::vec3 _size, const char* objPath, std::vector<glm::vec3> _colors, const char* texturePath) : position(_position), size(_size), colors(_colors)
{
	loadOBJ(objPath, loadVertices, loadUV, loadNormals);
	indexVBO(loadVertices, loadUV, loadNormals, indices, verticesIndex, uvsIndex, normalsIndex);
	VAO(programID);
	texturing(programID, texturePath);
	VBO();
	model = glm::translate(glm::mat4(1.0f), position);
}
Mesh::Mesh(GLuint programID, glm::vec3 _position, glm::vec3 _size, std::vector<glm::vec3> _vertices, int verticeSize, std::vector<glm::vec3> _colors, std::vector<glm::vec2>  _uvs, const char* texturePath) : position(_position), size(_size), colors(_colors)
{
	for (int i = 0; i < verticeSize; i += 3) {
		// Cr�ation des faces
		glm::vec3 v1 = glm::vec3(_vertices[i + 1].x - _vertices[i].x, _vertices[i + 1].y - _vertices[i].y, _vertices[i + 1].z - _vertices[i].z);
		glm::vec3 v2 = glm::vec3(_vertices[i + 2].x - _vertices[i].x, _vertices[i + 2].y - _vertices[i].y, _vertices[i + 2].z - _vertices[i].z);
		// Calcul de la normale du triangle
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
		// On le met 3 fois pour le nombre de points
		for (int z = 0; z < 3; z++)loadNormals.push_back(normal);
	}
	indexVBO(_vertices, _uvs, loadNormals, indices, verticesIndex, uvsIndex, normalsIndex);
	VAO(programID);
	texturing(programID, texturePath);
	VBO();
	model = glm::translate(glm::mat4(1.0f), position);
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
void Mesh::VBO()
{
	//indices
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	//Couleur
	//CreateArrayVBO(colorBuffer, colors.size(), colors[0]);
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size(), &colors[0], GL_STATIC_DRAW);
	//Vertex
	CreateArrayVBO(vertexBuffer, verticesIndex.size() * sizeof(glm::vec3), verticesIndex[0]);
	//UV
	CreateArrayVBO(uvBuffer, uvsIndex.size() * sizeof(glm::vec2), uvsIndex[0]);
	//Normal
	CreateArrayVBO(normalBuffer, normalsIndex.size() * sizeof(glm::vec3), normalsIndex[0]);
}

void Mesh::CreateArrayVBO(GLuint buffer, int size,glm::vec2 elem) {
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(unsigned short), &elem, GL_STATIC_DRAW);
}

void Mesh::CalculateMVP(glm::mat4 projection, glm::mat4 view)
{ 
	model = glm::scale(model, size);
	MVP = projection * view * model;
}

void Mesh::Draw(glm::vec3 camPos)
{
	glUniform3fv(lightPosID, 1, &camPos[0]);
	glUniform3fv(viewPosID, 1, &camPos[0]);
	glUniform1i(isTextureID, isTexture);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
 	//tampon Vertex
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//tampon Color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//tampo UV
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//tampo Normal
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//text
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
	//Disable For memory
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}