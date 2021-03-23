#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

/*
* Classe qui représente un cube du projet de base
*/
class Cube {

private:
	glm::vec3 pos, color; // Respectivement la position et la couleur du cube
	float rot; // La rotation du cube
	glm::mat4 model; // Le model de transformation du cube

public:
	Cube(glm::vec3 _pos, float _rot, glm::vec3 _color);
	~Cube();

	inline const glm::vec3& getPos() const { return pos; };
	inline const glm::vec3& getColor() const { return color; };
	inline const float& getRot() const { return rot; };
	inline const glm::mat4& getModel() const { return model;  }
};