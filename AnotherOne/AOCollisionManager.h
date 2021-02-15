#pragma once

#include <vector>

#include "AOObject.h"

class AOCollisionManager
{
public:
	static void resolveCollisions(std::vector<AOObject*> *objList);
};

