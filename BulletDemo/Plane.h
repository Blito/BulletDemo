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
	static bool load(GLuint shaderProgram);

	/**
	 * Constructor.
	 * @param y The y coordinate of the plane.
	 * @param width The width of the plane.
	 * @param depth The depth of the plane.
	 */
	Plane(int y = 0, unsigned width = 100, unsigned depth = 100);
	void render(const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & preMult);
	bool addToWorld(btDynamicsWorld * world);

private:
	static GLuint vbo;
	static GLint posAttrib;
	static GLint colAttrib;
	static GLuint sm_shaderProgram;

	btRigidBody * rigidBody; //< Bullet's internal representation of the plane
	int y;
	float width, depth; //< For rendering, not physics.

	// Fragment shader variables
	static GLint uniPVM, uniModel;

	glm::mat4 model; //< local transformation matrix
};

