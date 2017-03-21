#include "Scene.h"
#include "SceneParameters.h"
#include "Libraries\glm\gtc\matrix_transform.hpp"
#include "Libraries\glew\glew.h"
#include "MassOnString.h"
#include "ChainPendulum.h"
#include "CubeOfJelly.h"
#include "HangingCloth.h"
#include "Flag.h"
#include "ClothOnTable.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::initializeScene() {
	//define the projection matrix (it's constant and never changes)
	projectionMatrix = glm::perspective(
		glm::radians(90.0f),
		1.0f * SceneParameters::getScreenWidth() / SceneParameters::getScreenHeight(),
		SceneParameters::getZNear(),
		SceneParameters::getZFar()
	);

	//define the inverse of the projection matrix
	inverseProjectionMatrix = glm::inverse(projectionMatrix);
}

void Scene::renderScene(float dt) {
	//update the camera's position
	cam.update(dt);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(&projectionMatrix[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(&cam.getViewMatrix()[0][0]);

	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_QUADS);
	glVertex3f(-10, -5, -10);
	glVertex3f(10, -5, -10);
	glVertex3f(10, -5, 10);
	glVertex3f(-10, -5, 10);
	glEnd();

	if (obj != NULL) {
		obj->update(dt / 5.0);
		obj->update(dt / 5.0);
		obj->update(dt / 5.0);
		obj->update(dt / 5.0);
		obj->update(dt / 5.0);
		obj->draw(cam.getViewMatrix());
	}
}

glm::mat4 Scene::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Scene::getInverseProjectionMatrix() {
	return inverseProjectionMatrix;
}

void Scene::createMassOnSpring() {
	if (obj != NULL) {
		delete obj;
	}
	obj = new MassOnString();
}

void Scene::createChainPendulum() {
	if (obj != NULL) {
		delete obj;
	}
	obj = new ChainPendulum();
}

void Scene::createCubeOfJelly() {
	if (obj != NULL) {
		delete obj;
	}
	obj = new CubeOfJelly();
}

void Scene::createHangingCloth() {
	if (obj != NULL) {
		delete obj;
	}
	obj = new HangingCloth();
}

void Scene::createFlag() {
	if (obj != NULL) {
		delete obj;
	}
	obj = new Flag();
}

void Scene::createClothOnTable() {
	if (obj != NULL) {
		delete obj;
	}
	obj = new ClothOnTable();
}