#pragma once

#include <iostream>
#include <memory>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/ext/vector_float3.hpp>

#include "glengine/shader.hpp"
#include "particle.hpp"
#include "utils.hpp"



class ParticleSystem {


public:
	ParticleSystem(glm::vec3 pos, const Shader& _shader, int _texId);
	~ParticleSystem();

	void simulate(const double& dt, const glm::vec3& camPos);
	void render(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);

private:
	int texId;
	glm::vec3 pos;

	static const std::size_t max_particles = 500;
	std::size_t particle_count;

	GLuint vao;

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	GLuint particles_life_buffer;

	const Shader& shader;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
	GLfloat* g_particule_life_data;

	std::unique_ptr<Particle[]> particles;
	std::size_t last_used;

	void init();
	std::size_t findUnused();
	void sort();

};