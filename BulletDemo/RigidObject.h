#pragma once
#include "renderedobject.h"
#include <string>
#include <assimp/scene.h>   // (AssImp) Output data structure

class RigidObject :
	public RenderedObject
{
public:
	RigidObject(std::string& filename);
	~RigidObject(void);
	void render(glm::mat4 parentTransform);
	bool addToWorld(btDynamicsWorld * world);

protected:
	const aiScene * object;
	float *vertexArray;
	float *normalArray;
	float *uvArray;

	int numVerts;
};

