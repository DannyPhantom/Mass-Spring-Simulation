#include "MassOnString.h"
#include "Libraries/glew/glew.h"
#include "Libraries/freeglut/freeglut.h"
#include "Libraries/glm/gtx/transform.hpp"



MassOnString::MassOnString()
{
	create();
}


MassOnString::~MassOnString()
{
}

void MassOnString::create() {
	Mass *mass = new Mass(1.0, glm::vec3(0, 1, 0));
	mass->makeStatic();
	Mass *mass2 = new Mass(5.0, glm::vec3(0, 0, 0));
	Spring *spring = new Spring(50, glm::length(mass2->getPos() - mass->getPos()), 0.2, true);
	mass2->connectToMass(mass, spring);
	mass->connectToMass(mass2, spring);

	masses.push_back(mass);
	masses.push_back(mass2);
	springs.push_back(spring);
}

void MassOnString::draw(glm::mat4 viewMatrix) {
	Object::draw(viewMatrix);

	glLoadIdentity();
	glLoadMatrixf(&(viewMatrix * glm::translate(glm::mat4(), masses[1]->getPos()))[0][0]);
	glutSolidSphere(0.13, 20, 20);
}
