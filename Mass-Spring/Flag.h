#pragma once

#include "Object.h"

class Square {
private:
	Mass *m1;
	Mass *m2;
	Mass *m3;
	Mass *m4;

	glm::vec3 getNormal();
	glm::vec3 getNormalForce(glm::vec3 windDir, float windStrength);
	glm::vec3 getTangentialForce(glm::vec3 windDir, float windStrength);
	float getArea();
public:
	Square(Mass *m1, Mass *m2, Mass *m3, Mass *m4);
	~Square();

	void setWindForces(glm::vec3 windDir, float windStrength);
};

class Flag : public Object
{
private:
	glm::vec3 windPos;
	glm::vec3 windTarget;
	float windStrength;
	std::vector<Square *> squares;

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

