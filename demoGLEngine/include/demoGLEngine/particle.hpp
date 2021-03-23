#pragma once

#include <glm/ext/vector_float3.hpp>

/*
* Structure représentant une particule
*/
struct Particle {
	glm::vec3 pos, speed; // Respectivement la position et la vitesse de la particule
	float size; // Taille de la particule
	float life, maxLife; // Repectivement la vie actuelle et la vie maximum de la particule
	float cameraDistance; // Distance de la particule par rapport a la caméra

	Particle(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _speed = glm::vec3(0.0f, 1.0f, 0.0f), float _life = -10.f, float _maxLife = 1.0f) : pos(_pos), speed(_speed), life(_life), maxLife(_maxLife), cameraDistance(0), size(1) {};

	/*
	* Operateur de comparaison des particules en fonction de leur distance a la caméra
	*/
	bool operator<(Particle& that) {
		return this->cameraDistance > that.cameraDistance;
	}
};