#pragma once

#include "Object.h"

class MassOnString : public Object
{
public:
	MassOnString();
	~MassOnString();

	void create();
	void draw(glm::mat4 viewMatrix);
};

