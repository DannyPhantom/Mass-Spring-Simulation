#include "Flag.h"
#include <stdlib.h>     /* srand, rand */
#include "Libraries/glew/glew.h"
#include "Libraries/freeglut/freeglut.h"
#include "Libraries/glm/gtx/transform.hpp"
#include <iostream>



Flag::Flag()
{
	create();
	windPos = glm::vec3(randomFloat(-4, 4), randomFloat(0, 3), randomFloat(-2, 2));
	windStrength = randomFloat(0, 50);
}


Flag::~Flag()
{
}

void Flag::create() {
	const int numOfConnectionsY = 7;
	const int numOfConnectionsZ = 15;
	const double stepY = 2.0f / numOfConnectionsY;
	const double stepZ = 5.0f / numOfConnectionsZ;

	for (int i = 0; i < numOfConnectionsY; i++) {
		for (int j = 0; j < numOfConnectionsZ; j++) {
			Mass *mass = new Mass(0.2, glm::vec3(0, 3 + i * stepY, -1 + j * stepZ));
			masses.push_back(mass);
			if (j == 0) {
				mass->makeStatic();
			}
		}
	}

	for (Mass *m : masses) {
		for (Mass *m2 : masses) {
			if (m != m2) {
				double distance = glm::length(m2->getPos() - m->getPos());
				if (distance < 1.5 * stepZ) {
					double attenuation = (1 - (distance - stepZ) / (1.5 * stepZ));
					Spring *s = new Spring(200 * attenuation, distance, 0.008, false);
					if (distance < 1.1 * stepZ) {
						s->setVisible();
					}
					m2->connectToMass(m, s);
					m->connectToMass(m2, s);
					springs.push_back(s);
				}
			}
		}
	}

}

void Flag::update(float dt) {
	updateWind(dt);

	for (Mass *m : masses) {
		if (!m->getIsStatic()) {
			glm::vec3 nextPos = m->getPos();
			glm::vec3 numerator = glm::vec3(0.0f);
			for (std::pair<Mass*, Spring*> connectedMass : m->getConnectedMasses()) {
				Mass *mass = connectedMass.first;
				Spring *spring = connectedMass.second;
				numerator = numerator - (float)(spring->getK() * (glm::length(m->getPos() - mass->getPos()) - spring->getX0())) * glm::normalize(m->getPos() - mass->getPos()) - (float)spring->getDamping() * m->getVelocity();
			}
			glm::vec3 windDir = glm::normalize(m->getPos() - windPos);
			glm::vec3 acceleration = numerator / (float)m->getMass() - glm::vec3(0, 9.8, 0) + windStrength * windDir;
			m->setVelocity(m->getVelocity() + acceleration * dt);
			nextPos += m->getVelocity() * dt;
			if (nextPos.y <= -5) {
				nextPos.y = -5;
			}
			m->setNextPos(nextPos);
		}
	}

	for (Mass *m : masses) {
		m->applyNextPos();
	}
}

float Flag::randomFloat(float min, float max) {
	return min + (float) (rand()) / ((float) (RAND_MAX / (max - min)));
}

void Flag::updateWind(float dt) {
	glm::vec3 windPosChange = glm::vec3(randomFloat(-20, 20), randomFloat(-20, 20), randomFloat(-20, 20));
	float windStrengthChange = randomFloat(-45, 50);

	windPos += windPosChange * dt;
	windStrength += windStrengthChange * dt;

	windPos = clampVec3(windPos, glm::vec3(-1, 2, -4), glm::vec3(1, 4, -2));
	windStrength = clampFloat(windStrength, 0, 50);

	if (windStrength > 45) {
		windStrength = 19;
	}
}


float Flag::clampFloat(float val, float min, float max) {
	if (val < min) {
		val = min;
	}
	else if (val > max) {
		val = max;
	}

	return val;
}

glm::vec3 Flag::clampVec3(glm::vec3 val, glm::vec3 min, glm::vec3 max) {
	val.x = clampFloat(val.x, min.x, max.x);
	val.y = clampFloat(val.y, min.y, max.y);
	val.z = clampFloat(val.z, min.z, max.z);
	return val;
}

void Flag::draw(glm::mat4 viewMatrix) {
	Object::draw(viewMatrix);

	glColor3f(1, 0, 0);

	glLoadIdentity();
	glLoadMatrixf(&(viewMatrix * glm::translate(glm::mat4(), glm::vec3(0, 5, -1)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)))[0][0]);
	glutSolidCylinder(0.05, 20, 20, 20);

}
