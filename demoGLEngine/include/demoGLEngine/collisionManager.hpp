#pragma once

#include <cmath>
#include "target.hpp"
#include "bulletManager.hpp"
#include "cube.hpp"

class CollisionManager {

private:


public:
	CollisionManager();
	~CollisionManager();
	void testTargetCollisions(Target*, BulletManager*, const std::vector<Cube>&);

};