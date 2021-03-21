#pragma once

#include "target.hpp"
#include "bulletManager.hpp"

class CollisionManager {

private:


public:
	CollisionManager();
	~CollisionManager();
	void testTargetCollisions(Target*, BulletManager*);

};