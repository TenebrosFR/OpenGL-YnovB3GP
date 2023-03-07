#include "utils.h"

VEC3 Multiply(VEC3 vec, float value) {
	return VEC3(vec.x * value, vec.y * value, vec.z * value);
}