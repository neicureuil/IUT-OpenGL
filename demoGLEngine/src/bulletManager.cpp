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
	
	std::vector<GLfloat> vertices;

	int lats = 20;
	int longs = 20;

	for (int i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = SPHERE_RADIUS * sin(lat0);
		double zr0 = cos(lat0);
	
		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = SPHERE_RADIUS * sin(lat1);
		double zr1 = cos(lat1);

		for (int j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = SPHERE_RADIUS * cos(lng);
			double y = SPHERE_RADIUS * sin(lng);

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);

			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);
		}
	}

	verticeCount = vertices.size();

	glGenBuffers(1, &bulletVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

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
			b.pos += b.dir * b.speed * (float)dt;

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

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, verticeCount, bulletNumber);
}

void BulletManager::addBullet(glm::vec3 pos, glm::vec3 dir) {
	if (bulletNumber >= MAX_BULLETS) return;
	bullets[bulletNumber].pos = pos;
	bullets[bulletNumber].dir = dir;
	bullets[bulletNumber].life = 5.0f;
	bullets[bulletNumber].color = glm::vec3(Utils::random(), Utils::random(), Utils::random());
}

void BulletManager::removeBullet(size_t id) {
	if (id >= MAX_BULLETS) return;
	bullets[id].life = -1.0f;
}