#include "demoGLEngine/bulletManager.hpp"

BulletManager::BulletManager(const Shader& _shader) : shader(_shader) {
	bullets = new Bullet[MAX_BULLETS];
	g_buller_positions = new GLfloat[MAX_BULLETS * 3];

	init();
}

BulletManager::~BulletManager() {
	delete[] bullets;
	delete[] g_buller_positions;
}

void BulletManager::init() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};
	bulletVertices = 4;
	
	glGenBuffers(1, &bulletVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &bulletPositions);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPositions);
	glBufferData(GL_ARRAY_BUFFER, MAX_BULLETS * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPositions);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
}

void BulletManager::update(double dt) {
	bulletNumber = 0;
	for (size_t i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].life > 0) {
			Bullet& b = bullets[i];
			b.life -= dt;
			b.pos += b.dir * (float)dt;

			g_buller_positions[3 * bulletNumber + 0] = b.pos.x;
			g_buller_positions[3 * bulletNumber + 1] = b.pos.y;
			g_buller_positions[3 * bulletNumber + 2] = b.pos.z;
			
			bulletNumber++;
		}
	}

	std::sort(&bullets[0], &bullets[MAX_BULLETS]);
}

void BulletManager::render(glm::mat4 view, glm::mat4 proj) {
	shader.bind();
	shader.setMat4("view", view);
	shader.setMat4("projection", proj);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, bulletPositions);
	glBufferData(GL_ARRAY_BUFFER, MAX_BULLETS * 3 * sizeof(GLfloat), g_buller_positions, GL_STREAM_DRAW);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, bulletVertices, bulletNumber);
}

void BulletManager::addBullet(glm::vec3 pos, glm::vec3 dir) {
	bullets[bulletNumber].pos = pos;
	bullets[bulletNumber].dir = dir;
	bullets[bulletNumber].life = 5.0f;
}