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
	windTarget = glm::vec3(randomFloat(-4, 4), randomFloat(2, 5), randomFloat(3, 6));
	windStrength = randomFloat(0, 10);
}


Flag::~Flag()
{
}

void Flag::create() {
	const int numOfConnectionsY = 7;
	const int numOfConnectionsZ = 15;
	const double stepY = 2.0f / numOfConnectionsY;
	const double stepZ = 5.0f / numOfConnectionsZ;

	Mass ***grid = new Mass**[numOfConnectionsY];
	for (int i=0; i<numOfConnectionsY; i++) {
		grid[i] = new Mass*[numOfConnectionsZ];
	}

	for (int i = 0; i < numOfConnectionsY; i++) {
		for (int j = 0; j < numOfConnectionsZ; j++) {
			Mass *mass = new Mass(0.2, glm::vec3(0, 3 + i * stepY, -1 + j * stepZ));
			grid[i][j] = mass;
			masses.push_back(mass);
			if (j == 0) {
				mass->makeStatic();
			}
		}
	}

	for (int i = 0; i < numOfConnectionsY-1; i++) {
		for (int j = 0; j < numOfConnectionsZ-1; j++) {
			Square *sq = new Square(grid[i][j], grid[i+1][j], grid[i+1][j+1], grid[i][j+1]);
			squares.push_back(sq);
		}
	}

	for (Mass *m : masses) {
		for (Mass *m2 : masses) {
			if (m != m2) {
				double distance = glm::length(m2->getPos() - m->getPos());
				if (distance < 2.3 * stepZ) {
					double attenuation = (1 - (distance - stepZ) / (2.3 * stepZ));
					Spring *s = new Spring(300 * attenuation, distance, 0.015, false);
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
			glm::vec3 acceleration = (numerator + m->getWindForce()) / (float)m->getMass() - glm::vec3(0, 9.8, 0);
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
	glm::vec3 windPosChange = glm::vec3(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
	glm::vec3 windTargetChange = glm::vec3(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
	float windStrengthChange = randomFloat(-8, 10);

	windPos += windPosChange * dt;
	windTarget += windTargetChange * dt;
	windStrength += windStrengthChange * dt;

	windPos = clampVec3(windPos, glm::vec3(-1, 2, -4), glm::vec3(1, 4, -2));
	windTarget = clampVec3(windTarget, glm::vec3(-4, 2, 3), glm::vec3(4, 5, 6));
	windStrength = clampFloat(windStrength, 0, 8);

	if (windStrength > 7.5f) {
		windStrength = randomFloat(3, 6);
	}

	for (Mass *m : masses) {
		m->resetWindForce();
	}

	for (Square *sq : squares) {
		sq->setWindForces(windTarget - windPos, windStrength);
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

	glLoadIdentity();
	glLoadMatrixf(&(viewMatrix * glm::translate(glm::mat4(), windTarget))[0][0]);
	glColor3f(0.5, 0.9, 0.3);
	glutSolidSphere(0.03, 20, 20);

}

Square::Square(Mass *m1, Mass *m2, Mass *m3, Mass *m4) {
	this->m1 = m1;
	this->m2 = m2;
	this->m3 = m3;
	this->m4 = m4;
}

Square::~Square() {

}

glm::vec3 Square::getNormal() {
	return glm::normalize(glm::cross(m1->getPos()-m2->getPos(), m3->getPos()-m2->getPos()));
}

glm::vec3 Square::getNormalForce(glm::vec3 windDir, float windStrength) {
	float alpha = 0.8;
	float area = getArea();
	glm::vec3 squareNormal = getNormal();
	glm::vec3 wind = windDir * windStrength;
	//project wind onto normal
	glm::vec3 windNormal = glm::dot(wind, squareNormal) * squareNormal;
	return alpha * area * windStrength * windNormal;
}

glm::vec3 Square::getTangentialForce(glm::vec3 windDir, float windStrength) {
	float alpha = 0.5;
	float area = getArea();
	glm::vec3 squareNormal = getNormal();
	glm::vec3 wind = windDir * windStrength;

	//project wind onto tangent
	glm::vec3 windNormal = glm::dot(wind, squareNormal) * squareNormal;
	glm::vec3 windTangent = wind - windNormal;
	return alpha * area * windTangent;
}

float Square::getArea() {
	return glm::length(glm::cross(m1->getPos()-m2->getPos(), m3->getPos()-m2->getPos()));
}

void Square::setWindForces(glm::vec3 windDir, float windStrength) {
	glm::vec3 totalForce = getNormalForce(windDir, windStrength) + getTangentialForce(windDir, windStrength);
	glm::vec3 forcePerMass = totalForce / 4.0f;
	m1->addWindForce(forcePerMass);
	m2->addWindForce(forcePerMass);
	m3->addWindForce(forcePerMass);
	m4->addWindForce(forcePerMass);
}
