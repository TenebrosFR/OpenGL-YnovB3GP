#pragma once

VECTOR<Mesh*> meshesOfMap(GLuint programID) {
	VECTOR<Mesh*> meshes;
	//
	meshes.push_back(new Mesh(programID, VEC3(0,-2,0), VEC3(5,1,5), "cube.obj", "container.jpg")); // Plateforme de départ
	meshes.push_back(new Mesh(programID, VEC3(-12, -2, 0), VEC3(5, 1, 5), "cube.obj", "container.jpg")); //2 plateforme
	meshes.push_back(new Mesh(programID, VEC3(-15, 0, -3), VEC3(1, 1, 1), "cube.obj", "container.jpg")); //3 plateforme
	meshes.push_back(new Mesh(programID, VEC3(-15, 2, -1), VEC3(1, 1, 1), "cube.obj", "container.jpg")); //4 plateforme
	meshes.push_back(new Mesh(programID, VEC3(-13, 4, 0), VEC3(1, 1, 1), "cube.obj", "container.jpg")); //5 plateforme
	meshes.push_back(new Mesh(programID, VEC3(-11, 6, 1), VEC3(1, 1, 1), "cube.obj", "container.jpg")); //6 plateforme
	meshes.push_back(new Mesh(programID, VEC3(0, 7, 0), VEC3(5, 1, 5), "cube.obj", "container.jpg")); // 7


	//
	return meshes;
}