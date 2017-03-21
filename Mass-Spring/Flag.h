#pragma once

#include "Object.h"

class Flag : public Object
{
private:
	glm::vec3 windPos;
	float windStrength;

	void updateWind(float dt);
	float randomFloat(float min, float max);
	float clampFloat(float val, float min, float max);
	glm::vec3 clampVec3(glm::vec3 val, glm::vec3 min, glm::vec3 max);
public:
	Flag();
	~Flag();


	void create();
	void update(float dt);
	void draw(glm::mat4 viewMatrix);
};

