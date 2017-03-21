#include "Object.h"
#include "Libraries\glew\glew.h"
#include "Libraries\freeglut\freeglut.h"
#include "Libraries\glm\gtx\transform.hpp"
#include <iostream>

Object::Object()
{
}


Object::~Object()
{
	for (Mass *m : masses) {
		delete m;
	}

	for (Spring *s : springs) {
		delete s;
	}
}

void Object::draw(glm::mat4 viewMatrix) {
	for (Mass *m : masses) {
		for (std::pair<Mass *, Spring *> connectedM : m->getConnectedMasses()) {
			if (connectedM.second->getIsVisible()) {
				glColor3f(0, 1, 0);
				glLoadIdentity();
				glLoadMatrixf(&viewMatrix[0][0]);
				glBegin(GL_LINES);
				glVertex3f(m->getPos().x, m->getPos().y, m->getPos().z);
				glVertex3f(connectedM.first->getPos().x, connectedM.first->getPos().y, connectedM.first->getPos().z);
				glEnd();

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

void Object::update(float dt) {
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