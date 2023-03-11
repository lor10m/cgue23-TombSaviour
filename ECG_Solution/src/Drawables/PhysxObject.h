#pragma once
#include <iostream>
#include "..\Utils.h"
#include <vector>
#include <PxPhysicsAPI.h>

class PhysxObject{


private:
	GLuint VAO;
	GLuint VBO;
	GLuint NormalsVBO;
	GLuint EBO;

public:
	PhysxObject();
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	void initPhysics();
	void draw();
	void createObject(physx::PxMaterial* mMaterial, physx::PxPhysics* mPhysics);
	glm::vec3 getPosition();
};
