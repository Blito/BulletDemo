#pragma once
#include "RenderedObject.h"
#include "GL/glew.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

/**
 * Renderable object, a Box.
 * Has physics included, using Bullet Physics.
 */
class Box : public RenderedObject
{
public:
	static bool load();

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
	static const GLchar * vertexSource;
	static const GLchar * fragmentSource;
	btRigidBody * rigidBody;

	float i;

	// Fragment shader variables
	static GLint uniModel;
	static GLint uniView;
	static GLint uniProj;
};

