#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>

#include <algorithm>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "glengine/shader.hpp"
#include "bullet.hpp"

class BulletManager {

private:
	GLuint vao;
	GLuint bulletVertices;
	GLuint bulletPositions;
	size_t verticeCount;

	const double SPHERE_RADIUS = 0.5;

	const size_t MAX_BULLETS = 10;
	size_t bulletNumber = 0;

	Bullet* bullets = nullptr;
	GLfloat* g_buller_positions;

	const Shader& shader;

public:
	BulletManager(const Shader& _shader);
	~BulletManager();

	void init();

	void update(double);
	void render(glm::mat4 view, glm::mat4 proj);

	void addBullet(glm::vec3 pos, glm::vec3 dir);

};