#pragma once
#include <glm/ext/vector_float3.hpp>

class Cube {

private:
	glm::vec3 pos, color;
	float rot;

public:
	Cube(glm::vec3 _pos, float _rot, glm::vec3 _color);
	~Cube();

	inline const glm::vec3& getPos() const { return pos; };
	inline const glm::vec3& getColor() const { return color; };
	inline const float& getRot() const { return rot; };
};