#include "CubeOfJelly.h"



CubeOfJelly::CubeOfJelly()
{
	create();
}


CubeOfJelly::~CubeOfJelly()
{
}

void CubeOfJelly::create() {
	const int numOfConnections = 5;
	const double step = 2.0f / numOfConnections;

	for (int i = 0; i < numOfConnections; i++) {
		for (int j = 0; j < numOfConnections; j++) {
			for (int k = 0; k < numOfConnections; k++) {
				Mass *mass = new Mass(0.2, glm::vec3(-1 + i * step, -1 + j * step, -1 + k * step));
				masses.push_back(mass);
			}
		}
	}

	for (Mass *m : masses) {
		for (Mass *m2 : masses) {
			if (m != m2) {
				double distance = glm::length(m2->getPos() - m->getPos());
				if (distance < 1.5 * step) {
					double attenuation = (1 - (distance - step) / (1.5 * step));
					Spring *s = new Spring(80 * attenuation, distance, 0.012, false);
					if (distance < 2.1f / numOfConnections) {
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
