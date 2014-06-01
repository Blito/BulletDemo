#pragma once

#include "RenderedObject.h"
#include "GL/glew.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

/**
 * Renderable object, a horizontal plane.
 * Has physics included, using Bullet Physics.
 * Note: It's infinite simulation-wise, but has width+depth rendering-wise.
 */
class Plane : public RenderedObject
{
public:
	static bool load();

	/**
	 * Constructor.
	 * @param y The y coordinate of the plane.
	 * @param width The width of the plane.
	 * @param depth The depth of the plane.
	 */
	Plane(int y = 0, unsigned width = 100, unsigned depth = 100);
	void render(glm::mat4 parentTransform);
	bool addToWorld(btDynamicsWorld * world);

private:
	static const GLchar * vertexSource;
	static const GLchar * fragmentSource;

	btRigidBody * rigidBody; //< Bullet's internal representation of the plane
	int y;
	float width, depth; //< For rendering, not physics.

	// Fragment shader variables
	static GLint uniPVM;

	glm::mat4 model; //< local transformation matrix
};

