#include "demoGLEngine/target.hpp"
#include <iostream>

Target::Target(glm::vec3 _pos, const glMesh2& _mesh, const Shader& _shader)
	: pos(_pos), scale(glm::vec3(0.07f)), realPos(_pos), mesh(_mesh), shader(_shader), rot(72) {

	shader.bind();
	shader.setInt("textureTarget", 11);
	shader.setBool("hasDeformation", false);
}

Target::~Target() {

}

void Target::update(double dt) {
	// Change de la rotation de la cible
	rot += cos(dt);

	// Change de la rotation de la vitesse de la cible avec modulation
	double speed = 0.4 * sin(0.1 * dt) * 1 * cos(0.25 * dt);
	dt *= speed;


	// Change de la position de la cible => Forme de 8 avec modulation
	realPos.x = pos.x + (8 * sin(dt));
	realPos.z = pos.z + ((5 + 0.125 *sin(32*dt)) * cos(0.5*dt));

	// Update de la matrice de transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, realPos);
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
}

void Target::render(glm::mat4 view, glm::mat4 proj) {
	shader.bind();

	shader.setMat4("projection", proj);
	shader.setMat4("view", view);
	shader.setMat4("model", model);

	mesh.bind();
	mesh.render();
}

void Target::setDeformation(glm::vec2 pos, glm::vec3 dir, float speed, glm::vec3 color) {
	// Mise a jours des informations de la deformations sur la shader
	shader.bind();
	shader.setBool("hasDeformation", true);
	shader.setVec2("deformationPos", pos);
	shader.setVec3("deformationDir", dir);
	shader.setFloat("deformationSpeed", speed);
	shader.setVec3("deformationColor", color);
}