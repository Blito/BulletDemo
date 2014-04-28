#pragma once

#include "RenderedObject.h"

class Plane : public RenderedObject
{
public:
	Plane(int y);
	~Plane(void);

	void render();
	btRigidBody * getRigidBody() { return rigidBody; }

private:
	btRigidBody * rigidBody;
	int y;
};

