#pragma once

#include <cmath>
#include "target.hpp"
#include "bulletManager.hpp"
#include "cube.hpp"

/*
* Classe qui g�re les collisions
*/
class CollisionManager {

private:


public:
	CollisionManager();
	~CollisionManager();
	/*
	* Fonction qui detecte les collisions entre les balles et la cible ainsi que entre les balles et les cubes
	*/
	void testTargetCollisions(Target*, BulletManager*, const std::vector<Cube>&);

};