#pragma once

#include <bullet\BulletDynamics\Dynamics\btRigidBody.h>

class RenderedObject
{
public:
	virtual void render() = 0;
	virtual btRigidBody * getRigidBody() = 0;
};

