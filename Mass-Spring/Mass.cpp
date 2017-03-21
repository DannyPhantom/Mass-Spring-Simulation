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