#include "demoGLEngine/target.hpp"

Target::Target(glm::vec3 _pos, const glMesh& _mesh, const Shader& _shader)
 : pos(_pos), mesh(_mesh), shader(_shader) {


	shader.bind();
	shader.setInt("textureTarget", 11);
}

Target::~Target() {

}

void Target::render(glm::mat4 view, glm::mat4 proj) {
	shader.bind();

	shader.setMat4("projection", proj);
	shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(0.07f));
	shader.setMat4("model", model);

	mesh.bind();
	mesh.render();
}