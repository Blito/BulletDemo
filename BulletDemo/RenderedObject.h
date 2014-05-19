#pragma once
#include <bullet\BulletDynamics\Dynamics\btDynamicsWorld.h>
#include <bullet\BulletDynamics\Dynamics\btRigidBody.h>

/**
 * Interface for renderable objects.
 */
class RenderedObject
{
public:
	/**
	 * Render the object.
	 */
	virtual void render() = 0;

	/**
	 * Add this object to the physics world.
	 */
	virtual bool addToWorld(btDynamicsWorld * world) = 0;
};

