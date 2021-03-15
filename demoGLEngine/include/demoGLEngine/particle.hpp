#pragma once

#include <glm/ext/vector_float3.hpp>

struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a;
	float size, angle, weight;
	float life, maxLife;
	float cameraDistance;

	Particle(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _speed = glm::vec3(0.0f, 1.0f, 0.0f), float _life = -10.f, float _maxLife = 1.0f) : pos(_pos), speed(_speed), life(_life), maxLife(_maxLife) {};

	bool operator<(Particle& that) {
		return this->cameraDistance > that.cameraDistance;
	}
};