#include "HangingCloth.h"



HangingCloth::HangingCloth()
{
	create();
}


HangingCloth::~HangingCloth()
{
}

void HangingCloth::create() {
	const int numOfConnectionsX = 16;
	const int numOfConnectionsZ = 9;
	const double stepX = 5.0f / numOfConnectionsX;
	const double stepZ = 2.0f / numOfConnectionsZ;

	for (int i = 0; i < numOfConnectionsX; i++) {
		for (int j = 0; j < numOfConnectionsZ; j++) {
			Mass *mass = new Mass(0.2, glm::vec3(1 - i * stepX, 3, -1 + j * stepZ));
			masses.push_back(mass);
			if (i == 0) {
				mass->makeStatic();
			}
		}
	}

	for (Mass *m : masses) {
		for (Mass *m2 : masses) {
			if (m != m2) {
				double distance = glm::length(m2->getPos() - m->getPos());
				if (distance < 1.5 * stepX) {
					double attenuation = (1 - (distance - stepX) / (1.5 * stepX));
					Spring *s = new Spring(70 * attenuation, distance, 0.004, false);
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
