#pragma once

#include "RenderedObject.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

class Plane : public RenderedObject
{
public:
	Plane(int y);
	~Plane(void);

	void render();
	bool addToWorld(btDynamicsWorld * world);

private:
	btRigidBody * rigidBody;
	int y;
};

