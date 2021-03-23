#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>

struct Bullet {
	glm::vec3 pos, dir, color;
	float life, speed;

	Bullet() : pos(0,0,0), dir(0,0,0), color(0.5f,0.5f,0.5f), life(-10), speed(20) {};

	bool operator<(Bullet& that) {
		return this->life > that.life;
	}
};