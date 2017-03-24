#include "Mass.h"



Mass::Mass()
{
}


Mass::~Mass()
{
}


void Mass::connectToMass(Mass *mass, Spring *spring) {
	connectedMasses.push_back(std::make_pair(mass, spring));
}

void Mass::resetWindForce() {
	windForce = glm::vec3(0.0f);
}

void Mass::addWindForce(glm::vec3 force) {
	windForce += force;
}

glm::vec3 Mass::getWindForce() {
	return windForce;
}
