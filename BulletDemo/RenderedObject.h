#pragma once
#include <bullet\BulletDynamics\Dynamics\btDynamicsWorld.h>
#include <bullet\BulletDynamics\Dynamics\btRigidBody.h>

class RenderedObject
{
public:
	virtual void render() = 0;
	virtual bool addToWorld(btDynamicsWorld * world) = 0;
};

