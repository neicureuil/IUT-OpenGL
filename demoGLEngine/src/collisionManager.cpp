#include "demoGLEngine/collisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager() {

}

CollisionManager::~CollisionManager() {

}

void CollisionManager::testTargetCollisions(Target* target, BulletManager* bm, const std::vector<Cube>& cubes) {

	double checkRadius = 0.5;

	// Model de transformation de la cible qui sera appliquer aux points
	glm::mat4 model = target->getModel();

	// 3 Points de la cibles
	glm::vec3 tcenter = glm::vec3(model * glm::vec4(-49.5f, 49.5f, 0.0f, 1));
	glm::vec3 tp1 = glm::vec3(model * glm::vec4(49.5f, 49.5f, 0.0f, 1));
	glm::vec3 tp2 = glm::vec3(model * glm::vec4(-49.5f, -49.5f, 0.0f, 1));

	// Normale du plan de la cible
	glm::vec3 normal = Utils::vecProd( tp1 - tcenter, tp2 - tcenter );

	// Pour chaque des balles actives
	for (size_t i = 0; i < bm->getBulletNumber(); i++) {
		const Bullet& b = bm->getBullets()[i];

		// Test collision entre la balle et le plan de la cible
		if (abs(Utils::scalProd(normal, tp1 - b.pos)) / Utils::norme(normal) < 0.5f) {
			//Calcul des coords de la deformation sur la cible
			glm::vec3 c = glm::vec3(glm::inverse(model) * glm::vec4(b.pos, 1));
			float defX = (c.x + 49.5f) / (2.0f * 49.5f);
			float defY = (c.y + 49.5f) / (2.0f * 49.5f);

			// Si on est dans les coords de la cible 
			if (defX >= 0 && defX <= 1 && defY >= 0 && defY <= 1) {
				// Application de la deformation
				target->setDeformation(glm::vec2(defX, defY), b.dir, b.speed, b.color);
				// Suppression du bullet
				bm->removeBullet(i);
				// Comme on a une collision avec la cible, on peut ne pas tester la collision avec les cubes et passer a la balle suivante
				continue;
			}
		}

		// Tests de collision avec les cubes
		for (size_t j = 0; j < cubes.size(); j++) {
			glm::vec3 realPos = cubes[j].getModel() * glm::vec4(0.0f,0.0f,0.0f, 1.0f);
			if (glm::length(realPos - b.pos) < 1) {
				// Collision avec un cube
				bm->removeBullet(i);
				// Comme on a une collision avec un cube, on peut ne pas tester les autres et passer directement a la balle suivante
				break;
			}
		}
		
	}



}