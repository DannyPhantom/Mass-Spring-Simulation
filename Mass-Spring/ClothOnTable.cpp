#include "ClothOnTable.h"
#include "Libraries/glew/glew.h"
#include "Libraries/freeglut/freeglut.h"
#include "Libraries/glm/gtx/transform.hpp"



ClothOnTable::ClothOnTable()
{
	create();
}


ClothOnTable::~ClothOnTable()
{
}


void ClothOnTable::create() {
	const int numOfConnectionsX = 12;
	const int numOfConnectionsZ = 12;
	const double stepX = 7.0f / numOfConnectionsX;
	const double stepZ = 7.0f / numOfConnectionsZ;

	for (int i = 0; i < numOfConnectionsX; i++) {
		for (int j = 0; j < numOfConnectionsZ; j++) {
			Mass *mass = new Mass(0.2, glm::vec3(-3 + i * stepX, 3, -3 + j * stepZ));
			masses.push_back(mass);
		}
	}

	for (Mass *m : masses) {
		for (Mass *m2 : masses) {
			if (m != m2) {
				double distance = glm::length(m2->getPos() - m->getPos());
				if (distance < 1.5 * stepX) {
					double attenuation = (1 - (distance - stepX) / (1.5 * stepX));
					Spring *s = new Spring(170 * attenuation, distance, 0.02, false);
					if (distance < 1.1 * stepX) {
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

void ClothOnTable::drawTable(glm::mat4 viewMatrix) {
	glLoadIdentity();
	glLoadMatrixf(&viewMatrix[0][0]);

	//top
	glColor3f(0.7, 0.3, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-2, -0.03, -2);
	glVertex3f(2, -0.03, -2);
	glVertex3f(2, -0.03, 2);
	glVertex3f(-2, -0.03, 2);
	glEnd();

	//bottom
	glColor3f(0.1, 0.3, 0.1);
	glBegin(GL_QUADS);
	glVertex3f(-2, -1, -2);
	glVertex3f(2, -1, -2);
	glVertex3f(2, -1, 2);
	glVertex3f(-2, -1, 2);
	glEnd();

	//side1
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-2, -1, -2);
	glVertex3f(-2, -0.03, -2);
	glVertex3f(2, -0.03, -2);
	glVertex3f(2, -1, -2);
	glEnd();

	//side2
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-2, -1, 2);
	glVertex3f(-2, -0.03, 2);
	glVertex3f(2, -0.03, 2);
	glVertex3f(2, -1, 2);
	glEnd();

	//side3
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(2, -1, -2);
	glVertex3f(2, -0.03, -2);
	glVertex3f(2, -0.03, 2);
	glVertex3f(2, -1, 2);
	glEnd();

	//side4
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-2, -1, -2);
	glVertex3f(-2, -0.03, -2);
	glVertex3f(-2, -0.03, 2);
	glVertex3f(-2, -1, 2);
	glEnd();

	//leg1
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-2, -5, -2);
	glVertex3f(-2, -0.03, -2);
	glVertex3f(-1.8, -0.03, -2);
	glVertex3f(-1.8, -5, -2);
	glEnd();

	//leg2
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(-2, -5, 2);
	glVertex3f(-2, -0.03, 2);
	glVertex3f(-1.8, -0.03, 2);
	glVertex3f(-1.8, -5, 2);
	glEnd();

	//leg3
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(2, -5, -2);
	glVertex3f(2, -0.03, -2);
	glVertex3f(2, -0.03, -1.8);
	glVertex3f(2, -5, -1.8);
	glEnd();

	//leg4
	glColor3f(0.7, 0.8, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(2, -5, 2);
	glVertex3f(2, -0.03, 2);
	glVertex3f(2, -0.03, 1.8);
	glVertex3f(2, -5, 1.8);
	glEnd();
}

glm::vec3 ClothOnTable::crossesTableEdge(Mass *m1, Mass *m2) {
	if (m1->getPos().y >= 0 && m2->getPos().y >= 0 || m1->getPos().y <= -1 && m2->getPos().y <= -1) {
		return glm::vec3(0, 0, 0);
	}

	bool outsideInXDir = m1->getPos().x <= -2 && m2->getPos().x >= -2 || m2->getPos().x <= -2 && m1->getPos().x >= -2
		|| m1->getPos().x <= 2 && m2->getPos().x >= 2 || m2->getPos().x <= 2 && m1->getPos().x >= 2;

	bool outsideInZDir = m1->getPos().z <= 2 && m2->getPos().z >= 2 || m2->getPos().z <= 2 && m1->getPos().z >= 2
		|| m1->getPos().z <= -2 && m2->getPos().z >= -2 || m2->getPos().z <= -2 && m1->getPos().z >= -2;

	if (outsideInXDir && outsideInZDir) {
		return glm::vec3(0, 0, 0);
	}

	//edge 1
	if (m1->getPos().x <= -2 && m2->getPos().x >= -2 || m2->getPos().x <= -2 && m1->getPos().x >= -2) {
		float alpha = (-2 - m2->getPos().x) / (m1->getPos().x - m2->getPos().x);
		float z = alpha * m1->getPos().z + (1 - alpha) * m2->getPos().z;
		if (z > 2 || z < -2) return glm::vec3(0, 0, 0);
		return glm::vec3(-2, 0, z);
	}

	//edge 2
	if (m1->getPos().x <= 2 && m2->getPos().x >= 2 || m2->getPos().x <= 2 && m1->getPos().x >= 2) {
		float alpha = (2 - m2->getPos().x) / (m1->getPos().x - m2->getPos().x);
		float z = alpha * m1->getPos().z + (1 - alpha) * m2->getPos().z;
		if (z > 2 || z < -2) return glm::vec3(0, 0, 0);
		return glm::vec3(2, 0, z);
	}

	//edge 3
	if (m1->getPos().z <= 2 && m2->getPos().z >= 2 || m2->getPos().z <= 2 && m1->getPos().z >= 2) {
		float alpha = (2 - m2->getPos().z) / (m1->getPos().z - m2->getPos().z);
		float x = alpha * m1->getPos().x + (1 - alpha) * m2->getPos().x;
		if (x > 2 || x < -2) return glm::vec3(0, 0, 0);
		return glm::vec3(x, 0, 2);
	}

	//edge 3
	if (m1->getPos().z <= -2 && m2->getPos().z >= -2 || m2->getPos().z <= -2 && m1->getPos().z >= -2) {
		float alpha = (-2 - m2->getPos().z) / (m1->getPos().z - m2->getPos().z);
		float x = alpha * m1->getPos().x + (1 - alpha) * m2->getPos().x;
		if (x > 2 || x < -2) return glm::vec3(0, 0, 0);
		return glm::vec3(x, 0, -2);
	}

	return glm::vec3(0, 0, 0);
}


void ClothOnTable::draw(glm::mat4 viewMatrix) {
	drawTable(viewMatrix);
	for (Mass *m : masses) {
		for (std::pair<Mass *, Spring *> connectedM : m->getConnectedMasses()) {
			if (connectedM.second->getIsVisible()) {
				glColor3f(0, 1, 0);
				glLoadIdentity();
				glLoadMatrixf(&viewMatrix[0][0]);

				glm::vec3 crossPoint = crossesTableEdge(m, connectedM.first);
				if (crossPoint == glm::vec3(0, 0, 0)) {
					glBegin(GL_LINES);
					glVertex3f(m->getPos().x, m->getPos().y, m->getPos().z);
					glVertex3f(connectedM.first->getPos().x, connectedM.first->getPos().y, connectedM.first->getPos().z);
					glEnd();
				}
				else {
					glBegin(GL_LINES);
					glVertex3f(m->getPos().x, m->getPos().y, m->getPos().z);
					glVertex3f(crossPoint.x, crossPoint.y, crossPoint.z);
					glEnd(); 
					glBegin(GL_LINES);
					glVertex3f(crossPoint.x, crossPoint.y, crossPoint.z);
					glVertex3f(connectedM.first->getPos().x, connectedM.first->getPos().y, connectedM.first->getPos().z);
					glEnd();

					glLoadIdentity();
					glLoadMatrixf(&(viewMatrix * glm::translate(glm::mat4(), crossPoint))[0][0]);
					glColor3f(0.5, 0.9, 0.3);
					glutSolidSphere(0.03, 20, 20);
				}

				glLoadIdentity();
				glLoadMatrixf(&(viewMatrix * glm::translate(glm::mat4(), m->getPos()))[0][0]);
				glColor3f(0.5, 0.9, 0.3);
				glutSolidSphere(0.03, 20, 20);

				glLoadIdentity();
				glLoadMatrixf(&(viewMatrix * glm::translate(glm::mat4(), connectedM.first->getPos()))[0][0]);
				glutSolidSphere(0.03, 20, 20);
			}
		}
	}
}

glm::vec3 ClothOnTable::checkPos(glm::vec3 nextPos, glm::vec3 currentPos) {
	if (nextPos.y <= 0 && nextPos.x >= -2 && nextPos.x <= 2 && nextPos.z >= -2 && nextPos.z <= 2 && currentPos.y >= 0) {
		nextPos.y = 0;
	}
	else if (nextPos.y >= -1 && nextPos.x >= -2 && nextPos.x <= 2 && nextPos.z >= -2 && nextPos.z <= 2 && currentPos.y <= -1) {
		nextPos.y = -1;
	}
	else if (nextPos.y >= -1 && nextPos.y <= 0 && currentPos.y >= -1 && currentPos.y <= 0) {
		if (nextPos.x <= 2 && currentPos.x >= 2) {
			nextPos.x = 2;
		}
		if (nextPos.x >= -2 && currentPos.x <= -2) {
			nextPos.x = -2;
		}
		if (nextPos.z <= 2 && currentPos.z >= 2) {
			nextPos.z = 2;
		}
		if (nextPos.z >= -2 && currentPos.z <= -2) {
			nextPos.z = -2;
		}
	}

	return nextPos;
}

void ClothOnTable::update(float dt) {
	for (Mass *m : masses) {
		if (!m->getIsStatic()) {
			glm::vec3 nextPos = m->getPos();
			glm::vec3 numerator = glm::vec3(0.0f);
			for (std::pair<Mass*, Spring*> connectedMass : m->getConnectedMasses()) {
				Mass *mass = connectedMass.first;
				Spring *spring = connectedMass.second;
				numerator = numerator - (float)(spring->getK() * (glm::length(m->getPos() - mass->getPos()) - spring->getX0())) * glm::normalize(m->getPos() - mass->getPos()) - (float)spring->getDamping() * m->getVelocity();
			}
			glm::vec3 acceleration = numerator / (float)m->getMass() - glm::vec3(0, 9.8, 0);
			m->setVelocity(m->getVelocity() + acceleration * dt);
			nextPos += m->getVelocity() * dt;
			nextPos = checkPos(nextPos, m->getPos());
			m->setNextPos(nextPos);
		}
	}

	for (Mass *m : masses) {
		m->applyNextPos();
	}
}
