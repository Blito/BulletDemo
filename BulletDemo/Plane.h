#pragma once

#include "RenderedObject.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

/**
 * Renderable object, a horizontal plane.
 * Has physics included, using Bullet Physics.
 * Note: It's infinite simulation-wise, but has width+depth rendering-wise.
 */
class Plane : public RenderedObject
{
public:
	/**
	 * Constructor.
	 * @param y The y coordinate of the plane.
	 * @param width The width of the plane.
	 * @param depth The depth of the plane.
	 */
	Plane(int y = 0, unsigned width = 100, unsigned depth = 100);
	void render();
	bool addToWorld(btDynamicsWorld * world);

private:
	btRigidBody * rigidBody; //< Bullet's internal representation of the plane
	int y;
	unsigned width, depth; //< For rendering, not physics.
};

