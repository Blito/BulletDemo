#pragma once
#include "RenderedObject.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

/**
 * Renderable object, a Box.
 * Has physics included, using Bullet Physics.
 */
class Box : public RenderedObject
{
public:
	/**
	 * Constructor.
	 * @param mass If > 0, has physics interactions if added to a world.
	 */
	Box(float width, float height, float depth, float x = 0, float y = 0, float z = 0, float mass = 0);
	~Box(void);

	void render();
	btRigidBody * getRigidBody() { return rigidBody; }
	bool addToWorld(btDynamicsWorld * world);

private:
	btRigidBody * rigidBody;
};

