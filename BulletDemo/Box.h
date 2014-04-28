#pragma once
#include "RenderedObject.h"

class Box : public RenderedObject
{
public:
	Box(float width, float height, float depth, float x = 0, float y = 0, float z = 0, float mass = 0);
	~Box(void);

	void render();
	btRigidBody * getRigidBody() { return rigidBody; }

private:
	btRigidBody * rigidBody;
};

