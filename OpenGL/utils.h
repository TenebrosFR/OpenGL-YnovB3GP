#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/glm-master/glm/glm.hpp>
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/glm-master/glm/gtx/rotate_vector.hpp"
#include "C:/Tout/Cours/3eme annee/OpenGl/OpenGL-YnovB3GP/external/glm-master/glm/ext/matrix_transform.hpp"
#include <iostream>
//Define
#define VECTOR std::vector
#define VEC4 glm::vec4
#define VEC3 glm::vec3
#define VEC2 glm::vec2
#define MATRIX glm::mat4
#define TRANSLATE glm::translate
#define NORMALIZE glm::normalize
//
inline float EPSILON = 0.0001f;