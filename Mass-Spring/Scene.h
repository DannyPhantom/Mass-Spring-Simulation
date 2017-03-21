#pragma once

#include <vector>
#include "Camera.h"
#include <chrono>
#include <string>
#include "Object.h"

/*
	The main class of the program.
	Contains all the UI's, objects, basic controls, etc.
*/
class Scene
{
public:
	Scene();
	~Scene();

	/*
		Initializes the scene
	*/
	void initializeScene();

	/*
		Renders the scene
	*/
	void renderScene(float dt);

	/*
		Returns the scene's camera (controlled by the user)
	*/
	Camera *getCamera() { return &cam; }

	void createMassOnSpring();
	void createChainPendulum();
	void createCubeOfJelly();
	void createHangingCloth();
	void createFlag();
	void createClothOnTable();

private:

	/*
		Returns the projection matrix for the scene
	*/
	glm::mat4 getProjectionMatrix();

	/*
		Returns the inverse projection matrix for the scene
	*/
	glm::mat4 getInverseProjectionMatrix();

private:
	glm::mat4 projectionMatrix;
	glm::mat4 inverseProjectionMatrix;

	Camera cam;
	Object *obj;
};

