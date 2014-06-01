#pragma once
#include <bullet\BulletDynamics\Dynamics\btDynamicsWorld.h>
#include <bullet\BulletDynamics\Dynamics\btRigidBody.h>
#include <glm/glm.hpp>

/**
 * Interface for renderable objects.
 */
class RenderedObject
{
public:
	/**
	 * Render the object.
	 */
	virtual void render(glm::mat4 parentTransform) = 0;

	/**
	 * Add this object to the physics world.
	 */
	virtual bool addToWorld(btDynamicsWorld * world) = 0;
};

