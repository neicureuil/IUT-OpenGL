#include "demoGLEngine/collisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager() {

}

CollisionManager::~CollisionManager() {

}

void CollisionManager::testTargetCollisions(Target* target, BulletManager* bm, const std::vector<Cube>& cubes) {

	// Model de transformation de la cible qui sera appliquer aux points
	glm::mat4 model = target->getModel();

	// 3 Points de la cibles
	glm::vec3 tcenter = glm::vec3(model * glm::vec4(target->getMin().x, target->getMax().y, 0.0f, 1));
	glm::vec3 tp1 = glm::vec3(model * glm::vec4(target->getMax().x, target->getMax().y, 0.0f, 1));
	glm::vec3 tp2 = glm::vec3(model * glm::vec4(target->getMin().x, target->getMin().y, 0.0f, 1));

	// Normale du plan de la cible
	glm::vec3 normal = Utils::vecProd( tp1 - tcenter, tp2 - tcenter );

	// Pour chaque des balles actives
	for (size_t i = 0; i < bm->getBulletNumber(); i++) {
		const Bullet& b = bm->getBullets()[i];

		// Test collision entre la balle et le plan de la cible
		if (abs(Utils::scalProd(normal, tp1 - b.pos)) / Utils::norme(normal) < b.radius) {
			//Calcul des coords de la deformation sur la cible
			glm::vec3 c = glm::vec3(glm::inverse(model) * glm::vec4(b.pos, 1));
			float defX = (c.x + target->getMax().x) / (2.0f * target->getMax().x);
			float defY = (c.y + target->getMax().y) / (2.0f * target->getMax().y);

			// Si on est dans les coords de la cible  (entre 0 et 1 en x et en y)
			if (defX >= 0 && defX <= 1 && defY >= 0 && defY <= 1) {
				// Application de la deformation
				target->setDeformation(glm::vec2(defX, defY), b.dir, b.radius, b.color);
				// Suppression du bullet
				bm->removeBullet(i);
				// Comme on a une collision avec la cible, on peut ne pas tester la collision avec les cubes et passer a la balle suivante
				continue;
			}
		}

		// Tests de collision avec les cubes
		for (size_t j = 0; j < cubes.size(); j++) {
			glm::vec3 realPos = cubes[j].getModel() * glm::vec4(0.0f,0.0f,0.0f, 1.0f);
			if (glm::length(realPos - b.pos) < 0.5 + b.radius) {
				// Collision avec un cube
				bm->removeBullet(i);
				// Comme on a une collision avec un cube, on peut ne pas tester les autres et passer directement a la balle suivante
				break;
			}
		}
		
	}

}