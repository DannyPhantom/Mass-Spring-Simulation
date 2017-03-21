#include "ChainPendulum.h"



ChainPendulum::ChainPendulum()
{
	create();
}


ChainPendulum::~ChainPendulum()
{
}


void ChainPendulum::create() {
	const int numOfConnections = 8;

	Mass *mass = new Mass(1.0, glm::vec3(0, 1, 0));
	mass->makeStatic();
	masses.push_back(mass);

	for (int i = 0; i < numOfConnections; i++) {
		Mass *anotherMass = new Mass(0.5, glm::vec3(0, 1, 2.0f * (i+1) / numOfConnections));
		Spring *spring = new Spring(500, glm::length(anotherMass->getPos() - masses[i]->getPos()), 0.04, true);
		anotherMass->connectToMass(masses[i], spring);
		masses[i]->connectToMass(anotherMass, spring);
		masses.push_back(anotherMass);
		springs.push_back(spring);
	}

}