#include "demoGLEngine/target.hpp"
#include <iostream>

Target::Target(glm::vec3 _pos, const glMesh& _mesh, const Shader& _shader)
	: pos(_pos), scale(glm::vec3(0.07f)), realPos(_pos), mesh(_mesh), shader(_shader) {

	shader.bind();
	shader.setInt("textureTarget", 11);
	shader.setBool("hasDeformation", false);
}

Target::~Target() {

}

void Target::update(double dt) {
	realPos.x = pos.x + 8 * sin(dt);
	realPos.z = pos.z + (5 + 0.125 *sin(32*dt)) * cos(0.5*dt);
}

void Target::render(glm::mat4 view, glm::mat4 proj) {
	shader.bind();

	shader.setMat4("projection", proj);
	shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, realPos);
	model = glm::scale(model, scale);
	shader.setMat4("model", model);

	mesh.bind();
	mesh.render();
}

void Target::setDeformation(glm::vec2 pos, glm::vec3 dir) {
	shader.bind();
	shader.setBool("hasDeformation", true);
	shader.setVec2("deformationPos", pos);
	shader.setVec3("deformationDir", dir);
	std::cout << pos.x << std::endl;
}