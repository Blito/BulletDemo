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

	void render(const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & preMult);
	btRigidBody * getRigidBody() { return rigidBody; }
	bool addToWorld(btDynamicsWorld * world);

private:
	static GLuint vbo;
	static GLuint vao;
	static GLint posAttrib;
	static GLint colAttrib;
	static GLuint sm_shaderProgram;
    
	btRigidBody * rigidBody;

	float width, height, depth;

	// Fragment shader variables
	static GLint uniPVM, uniModel;

	glm::mat4 model; //< local transformation matrix
};

