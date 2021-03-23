#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>

/*
 * Structure représentant une balle
 */
struct Bullet {
	glm::vec3 pos, dir, color; // Respectivement la position, la direction et la couleur de la balle
	float life, speed; // Respectivement la vie de la balle et sa vitesse
	float radius; // Rayon de la balle

	Bullet() : pos(0,0,0), dir(0,0,0), color(0.5f,0.5f,0.5f), life(-10), speed(20), radius(0.5f) {};

	/* 
	* Operateur de comparaison pour trier les balles en fonction de leur vie
	*/
	bool operator<(Bullet& that) {
		return this->life > that.life;
	}
};