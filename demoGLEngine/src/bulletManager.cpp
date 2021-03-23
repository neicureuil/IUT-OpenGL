#include "demoGLEngine/bulletManager.hpp"

BulletManager::BulletManager(const Shader& _shader) : shader(_shader) {
	bullets = new Bullet[MAX_BULLETS];
	g_buller_positions = new GLfloat[MAX_BULLETS * 3];
	g_buller_colors = new GLfloat[MAX_BULLETS * 3];;

	init();
}

BulletManager::~BulletManager() {
	delete[] bullets;
	delete[] g_buller_positions;
	delete[] g_buller_colors;
}

void BulletManager::init() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	

	// Generations des vertices de la sphere (discrétisation)
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

	// Buffer des vertices
	glGenBuffers(1, &bulletVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	// Buffer des positions des balles
	glGenBuffers(1, &bulletPositions);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPositions);
	glBufferData(GL_ARRAY_BUFFER, MAX_BULLETS * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// Buffer des couleurs des balles
	glGenBuffers(1, &bulletColors);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColors);
	glBufferData(GL_ARRAY_BUFFER, MAX_BULLETS * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bulletPositions);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, bulletColors);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

void BulletManager::update(double dt) {
	bulletNumber = 0;
	for (size_t i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].life > 0) {

			Bullet& b = bullets[i];
			b.life -= dt;
			// Deplacement de la balle en fonction de sa direction et de sa vitesse
			b.pos += b.dir * b.speed * (float)dt;

			// Mise a jours de la position de la balle dans le buffer correspondant
			g_buller_positions[3 * bulletNumber + 0] = b.pos.x;
			g_buller_positions[3 * bulletNumber + 1] = b.pos.y;
			g_buller_positions[3 * bulletNumber + 2] = b.pos.z;

			// Mise a jours de la couleur de la balle dans le buffer correspondant
			g_buller_colors[3 * bulletNumber + 0] = b.color.x;
			g_buller_colors[3 * bulletNumber + 1] = b.color.y;
			g_buller_colors[3 * bulletNumber + 2] = b.color.z;
			
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

	glBindBuffer(GL_ARRAY_BUFFER, bulletColors);
	glBufferData(GL_ARRAY_BUFFER, MAX_BULLETS * 3 * sizeof(GLfloat), g_buller_colors, GL_STREAM_DRAW);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, verticeCount, bulletNumber);
}

void BulletManager::addBullet(glm::vec3 pos, glm::vec3 dir) {
	if (bulletNumber >= MAX_BULLETS) return;
	// Ajout d'une balle => Met a jours les informations d'une balle non-utilisé dans le tableau de balles
	bullets[bulletNumber].radius = BulletManager::SPHERE_RADIUS;
	bullets[bulletNumber].pos = pos;
	bullets[bulletNumber].dir = dir;
	bullets[bulletNumber].life = 5.0f;
	bullets[bulletNumber].color = glm::vec3(Utils::random(), Utils::random(), Utils::random());
}

void BulletManager::removeBullet(size_t id) {
	if (id >= MAX_BULLETS) return;
	// Supprime une balle en mettant sa vie a -1
	bullets[id].life = -1.0f;
}