#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>

struct Bullet {
	glm::vec3 pos, dir;
	float life, speed;

	Bullet() : pos(0,0,0), dir(0,0,0), life(-10), speed(20) {};
	Bullet(glm::vec3 _pos, glm::vec3 _dir) : pos(_pos), dir(_dir), life(5), speed(20) {};

	bool operator<(Bullet& that) {
		return this->life > that.life;
	}
};