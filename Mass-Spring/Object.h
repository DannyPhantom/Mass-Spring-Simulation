#pragma once

#include "Mass.h"
#include "Spring.h"
#include <vector>
#include "Libraries\glm\glm.hpp"

class Object
{
protected:
	std::vector<Mass *> masses;
	std::vector<Spring *> springs;
public:
	Object();
	virtual ~Object();

	virtual void create() = 0;
	virtual void draw(glm::mat4 viewMatrix);

	virtual void update(float dt);
};

