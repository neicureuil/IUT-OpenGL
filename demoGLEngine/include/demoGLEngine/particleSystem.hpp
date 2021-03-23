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



/*
* Classe qui repésente un systeme de particule dans la scene
*/
class ParticleSystem {


public:
	ParticleSystem(glm::vec3 pos, const Shader& _shader, int _texId);
	~ParticleSystem();

	void simulate(const double& dt, const glm::vec3& camPos);
	void render(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);

private:
	int texId; // Id de la texture sur le GPU
	glm::vec3 pos; // Position du systeme

	static const std::size_t max_particles = 500; // Nombre maximum de particules dans le systeme
	std::size_t particle_count; // Nombre actuel de particule dans le systeme

	GLuint vao;
	GLuint billboard_vertex_buffer; 
	GLuint particles_position_buffer;
	GLuint particles_life_buffer;

	const Shader& shader; // Shader de la particule

	GLfloat* g_particule_position_size_data;
	GLfloat* g_particule_life_data;

	std::unique_ptr<Particle[]> particles; // Tableau des particules
	std::size_t last_used; // Derniere particule utilisé

	/*
	* Fonction d'initialisation du systeme qui genere les differents buffers sur le GPU
	*/
	void init();
	/*
	* Fonction qui renvoie la position d'une particules non-utilisé dans le tableau de particules
	*/
	std::size_t findUnused();
	/*
	* Fonction qui trie les particules dans le tableau
	*/
	void sort();

};