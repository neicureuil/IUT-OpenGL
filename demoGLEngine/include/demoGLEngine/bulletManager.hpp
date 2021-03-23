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
#include "utils.hpp"

/*
* Classe gerant toutes les balles dans la scene
*/
class BulletManager {

private:
	GLuint vao;
	GLuint bulletVertices;
	GLuint bulletPositions;
	GLuint bulletColors;
	size_t verticeCount;

	const float SPHERE_RADIUS = 0.5;

	const size_t MAX_BULLETS = 20; // Nombre maximum de balles dans la scene
	size_t bulletNumber = 0;

	Bullet* bullets = nullptr; // Tableau de balles (nombre maximum => MAX_BULLETS)
	GLfloat* g_buller_positions;
	GLfloat* g_buller_colors;

	const Shader& shader;

public:
	BulletManager(const Shader& _shader);
	~BulletManager();

	void init();

	void update(double);
	void render(glm::mat4 view, glm::mat4 proj);

	void addBullet(glm::vec3 pos, glm::vec3 dir);
	void removeBullet(size_t id);

	inline const size_t getBulletNumber() { return bulletNumber; };
	inline const Bullet* getBullets() { return bullets; };

};