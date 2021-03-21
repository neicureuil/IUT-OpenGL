#include "demoGLEngine/collisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager() {

}

CollisionManager::~CollisionManager() {

}

void CollisionManager::testTargetCollisions(Target* target, BulletManager* bm) {

	double checkRadius = 0.1;

	// Points d'une diagonale de la boite de collision de la cible
	glm::vec3 targetUpLeft( target->getPos().x - (target->getSize().x * target->getScale().x)/2, target->getPos().y - (target->getSize().y * target->getScale().y)/2, target->getPos().z - checkRadius);
	glm::vec3 targetDownRight(target->getPos().x + (target->getSize().x * target->getScale().x) / 2, target->getPos().y + (target->getSize().y * target->getScale().y) / 2, target->getPos().z + checkRadius);

	for (int i = 0; i < bm->getBulletNumber(); i++) {

		double radius = 1;
		const Bullet& b = bm->getBullets()[i];

		if (b.pos.x > targetUpLeft.x && b.pos.y > targetUpLeft.y && b.pos.z > targetUpLeft.z  &&  b.pos.x < targetDownRight.x && b.pos.y < targetDownRight.y && b.pos.z < targetDownRight.z) {
			// Collision d'un bullet avec la target

			bm->removeBullet(i);
		}

	}

}