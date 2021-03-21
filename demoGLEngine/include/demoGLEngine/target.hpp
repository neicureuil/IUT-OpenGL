#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

#include "glengine/glmesh.hpp"
#include "glengine/shader.hpp"

class Target {

private:
	glm::vec3 pos;
	const Shader& shader;
	const glMesh& mesh;

public:
	Target(glm::vec3 _pos, const glMesh& _mesh, const Shader& _shader);
	~Target();

	void render(glm::mat4 view, glm::mat4 proj);

};