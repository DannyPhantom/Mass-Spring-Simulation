#pragma once

#include "Object.h"

class ClothOnTable : public Object
{
private:
	void drawTable(glm::mat4 viewMatrix);
	glm::vec3 checkPos(glm::vec3 nextPos, glm::vec3 currentPos);
	glm::vec3 crossesTableEdge(Mass *m1, Mass *m2);
public:
	ClothOnTable();
	~ClothOnTable();

	void create();
	void draw(glm::mat4 viewMatrix);

	void update(float dt);
};

