#include "demoGLEngine/cube.hpp"
#include <glm/ext/matrix_transform.hpp>

Cube::Cube(glm::vec3 _pos, float _rot, glm::vec3 _color)
	: pos(_pos), rot(_rot), color(_color) {
	// Definition du model de transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
}

Cube::~Cube() {

}