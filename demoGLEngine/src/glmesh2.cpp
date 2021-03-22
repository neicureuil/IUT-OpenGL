#include "demoGLEngine/glmesh2.hpp"
#include <iostream>

glMesh2::glMesh2()
	:meshVAO(0), meshVBO(0), positionVBO(0), meshEBO(0), texCoordVBO(0), normalVBO(0), indicesCount(0), verticesCount(0), usage(GL_STATIC_DRAW) {
}

glMesh2::glMesh2(unsigned int _verticesCount, unsigned int _indicesCount, GLenum _usage)
	: meshVAO(0), meshVBO(0), positionVBO(0), meshEBO(0), texCoordVBO(0), normalVBO(0), verticesCount(_verticesCount), indicesCount(_indicesCount), usage(_usage) {
}

glMesh2::glMesh2(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<unsigned int>& indices, GLenum _usage)
	: glMesh2(vertices.size() / 3, indices.size(), _usage) {
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	GLuint verticesBufferSize = verticesCount * 3.0 * sizeof(float);

	// Faces
	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

	// position attribute
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute	
	if (normals.size()) {
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), usage);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}

	// u,v attribute
	if (texCoords.size()) {
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), usage);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}
}


glMesh2::glMesh2(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, GLenum _usage)
	:glMesh2(vertices.size() / 3, indices.size(), _usage) {

	std::vector<float> newVertices;
	for (size_t i = 0; i < vertices.size() / 8; i++) {
		glm::vec3 pos1 = glm::vec3(vertices[i * 8 + 0], vertices[i * 8 + 1], vertices[i * 8 + 2]);
		glm::vec3 norm1 = glm::vec3(vertices[i * 8 + 3], vertices[i * 8 + 4], vertices[i * 8 + 5]);
		glm::vec2 uv1 = glm::vec2(vertices[i * 8 + 6], vertices[i * 8 + 7]);

		newVertices.push_back(pos1.x);
		newVertices.push_back(pos1.y);
		newVertices.push_back(pos1.z);
		newVertices.push_back(norm1.x);
		newVertices.push_back(norm1.y);
		newVertices.push_back(norm1.z);
		newVertices.push_back(uv1.x);
		newVertices.push_back(uv1.y);
		newVertices.push_back(0);
		newVertices.push_back(0);
		newVertices.push_back(0);
		newVertices.push_back(0);
		newVertices.push_back(0);
		newVertices.push_back(0);	
	}

	for (size_t i = 0; i < indices.size() / 3; i += 3) {
		size_t i1 = indices[i * 3] * 14;
		size_t i2 = indices[i * 3 + 1] * 14;
		size_t i3 = indices[i * 3 + 2] * 14;

		glm::vec3 pos1 = glm::vec3(newVertices[i1 + 0], newVertices[i1 + 1], newVertices[i1 + 2]);
		glm::vec3 pos2 = glm::vec3(newVertices[i2 + 0], newVertices[i2 + 1], newVertices[i2 + 2]);
		glm::vec3 pos3 = glm::vec3(newVertices[i3 + 0], newVertices[i3 + 1], newVertices[i3 + 2]);

		glm::vec3 norm1 = glm::vec3(newVertices[i1 + 3], newVertices[i1 + 4], newVertices[i1 + 5]);
		glm::vec3 norm2 = glm::vec3(newVertices[i2 + 3], newVertices[i2 + 4], newVertices[i2 + 5]);
		glm::vec3 norm3 = glm::vec3(newVertices[i3 + 3], newVertices[i3 + 4], newVertices[i3 + 5]);

		glm::vec2 uv1 = glm::vec2(newVertices[i1 + 6], newVertices[i1 + 7]);
		glm::vec2 uv2 = glm::vec2(newVertices[i2 + 6], newVertices[i2 + 7]);
		glm::vec2 uv3 = glm::vec2(newVertices[i3 + 6], newVertices[i3 + 7]);

		// Calcul des tangentes et des bi-tangentes
		glm::vec3 tangent, bitangent;

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		newVertices[i1 + 8] = tangent.x;
		newVertices[i1 + 9] = tangent.y;
		newVertices[i1 + 10] = tangent.z;
		newVertices[i1 + 11] = bitangent.x;
		newVertices[i1 + 12] = bitangent.y;
		newVertices[i1 + 13] = bitangent.z;
	}

	glGenVertexArrays(1, &meshVAO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(meshVAO);

	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

	glBufferData(GL_ARRAY_BUFFER, newVertices.size() * sizeof(float), newVertices.data(), usage);

	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);

	// normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

	// u,v attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));


	// Tangente, Bitangante attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

}


glMesh2::~glMesh2() {
	release();
}


glMesh2::glMesh2(glMesh2&& other) noexcept
	:meshVAO(other.meshVAO), meshVBO(other.meshVBO), positionVBO(other.positionVBO), meshEBO(other.meshEBO), texCoordVBO(other.texCoordVBO), normalVBO(other.normalVBO), indicesCount(other.indicesCount), verticesCount(other.verticesCount), usage(other.usage) {
	other.meshVAO = 0;
	other.meshVBO = 0;
	other.positionVBO = 0;
	other.meshEBO = 0;
	other.texCoordVBO = 0;
	other.normalVBO = 0;
}

glMesh2& glMesh2::operator=(glMesh2&& other) noexcept {
	//ALWAYS check for self-assignment.
	if (this != &other) {
		release();

		//obj_ is now 0.
		std::swap(meshVAO, other.meshVAO);
		std::swap(meshVBO, other.meshVBO);
		std::swap(meshEBO, other.meshEBO);
		std::swap(positionVBO, other.positionVBO);
		std::swap(texCoordVBO, other.texCoordVBO);
		std::swap(normalVBO, other.normalVBO);
		std::swap(indicesCount, other.indicesCount);
		std::swap(verticesCount, other.verticesCount);
		std::swap(usage, other.usage);
	}
	return *this;
}

void glMesh2::bind() const {
	glBindVertexArray(meshVAO);
}


void glMesh2::release() {
	glDeleteVertexArrays(1, &meshVAO);
	glDeleteBuffers(1, &meshVBO);
	glDeleteBuffers(1, &meshEBO);
	glDeleteBuffers(1, &positionVBO);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &normalVBO);

	meshVAO = 0;
	meshVBO = 0;
	positionVBO = 0;
	meshEBO = 0;
	texCoordVBO = 0;
	normalVBO = 0;
}