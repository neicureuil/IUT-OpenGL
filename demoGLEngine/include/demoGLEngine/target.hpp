#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

#include "glengine/glmesh.hpp"
#include "glengine/shader.hpp"

#include "glmesh2.hpp"
#include "bullet.hpp"

/*
* Classe qui represente une cible
*/
class Target {

private:
	float rot; // Rotation de la cible
	glm::vec3 pos, scale, realPos; // Respectivement la position dans la scene, la taille et la position apres deplacement de la cible 
	const Shader& shader; // Shader de la cible
	const glMesh2& mesh; // Meshs de la cible
	glm::mat4 model; // Matrice de transformation
	glm::vec3 min, max; /// Respectivement les coords minimum et maximum des vertices

public:
	Target(glm::vec3 _pos, const glMesh2& _mesh, const Shader& _shader, glm::vec3 _min, glm::vec3 _max);
	~Target();

	/*
	* Fonction qui demande le rendu de la cible
	*/
	void render(glm::mat4 view, glm::mat4 proj);
	/*
	* Fonction qui demande la mise a jours de la cible au cours du temps
	* (En particulier sa position)
	*/
	void update(double dt);
	/*
	* Fonction qui demande une deformation de la cible a un point précit.
	*/
	void setDeformation(glm::vec2 pos, glm::vec3 dir, float radius, glm::vec3 color);
	
	inline const glm::vec3& getPos() const { return realPos; };
	inline const glm::vec3& getScale() const { return scale; };
	inline const glm::vec2& getSize() const { return glm::vec2(100,100); };
	inline const float getRot() const { return rot; };
	inline const glm::mat4& getModel() const { return model; };
	inline const glm::vec3& getMin() const { return min; };
	inline const glm::vec3& getMax() const { return max; };
};