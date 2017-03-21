#pragma once

#include "Spring.h"
#include <vector>
#include "Libraries/glm/glm.hpp"

class Mass
{
private:
	double mass;
	std::vector<std::pair<Mass *, Spring*>> connectedMasses;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 nextPos;
	bool isStatic = false;

	glm::vec3 velocity = glm::vec3(0.0f);
public:
	Mass();
	Mass(double mass) : mass(mass) {}
	Mass(double mass, glm::vec3 pos) : mass(mass), pos(pos) {}
	~Mass();

	void setMass(double mass) { this->mass = mass; }
	double getMass() { return mass; }

	void setPos(glm::vec3 pos) { this->pos = pos; }
	glm::vec3 getPos() { return pos; }

	void makeStatic() { isStatic = true; }
	bool getIsStatic() { return isStatic; }

	void connectToMass(Mass *mass, Spring *spring);

	std::vector<std::pair<Mass *, Spring *>> getConnectedMasses() { return connectedMasses; }
	
	void setNextPos(glm::vec3 pos) { nextPos = pos; }
	void applyNextPos() { if (!isStatic) pos = nextPos; }

	glm::vec3 getVelocity() { return velocity; }
	void setVelocity(glm::vec3 vel) { velocity = vel; }

};

