#pragma once

#include <stdlib.h>
#include <glm/ext/vector_float3.hpp>

class Utils {

public:
	Utils() = delete;
	~Utils() = delete;

	static float random(int min, int max);
	static float random();
	static glm::vec3 vecProd(const glm::vec3& a, const glm::vec3& b);
	static float scalProd(const glm::vec3& a, const glm::vec3& b);
	static float norme(const glm::vec3& a);
};