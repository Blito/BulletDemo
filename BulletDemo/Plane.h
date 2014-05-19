#pragma once

#include "RenderedObject.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

/**
 * Renderable object, a horizontal plane.
 * Has physics included, using Bullet Physics.
 */
class Plane : public RenderedObject
{
public:
	/**
	 * Constructor.
	 * @param y The y coordinate of the plane.
	 */
	Plane(int y);

	void render();
	bool addToWorld(btDynamicsWorld * world);

private:
	btRigidBody * rigidBody; //< Bullet's internal representation of the plane
	int y;
};

