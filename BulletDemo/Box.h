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
	static bool load(GLuint shaderProgram);

	/**
	 * Constructor.
	 * @param mass If > 0, has physics interactions if added to a world.
	 */
	Box(float width, float height, float depth, float x = 0, float y = 0, float z = 0, float mass = 0);
	~Box(void);

	void render(glm::mat4 parentTransform);
	btRigidBody * getRigidBody() { return rigidBody; }
	bool addToWorld(btDynamicsWorld * world);

private:
	static GLuint vbo;
	static GLint posAttrib;
	static GLint colAttrib;
	static GLuint sm_shaderProgram;
    
	btRigidBody * rigidBody;

	float width, height, depth;

	// Fragment shader variables
	static GLint uniPVM;

	glm::mat4 model; //< local transformation matrix
};

