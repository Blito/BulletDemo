#pragma once
#include "renderedobject.h"
#include "GL/glew.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>
#include <bullet\BulletSoftBody\btSoftBodyHelpers.h>

/**
 * Renderable object, a cloth.
 * Has physics included, is modeled as a soft body using Bullet Physics.
 */
class Cloth :
	public RenderedObject
{
public:
	static bool load(GLuint shaderProgram);

	/**
	 * Constructor.
	 * Creates the cloth patch using a helper class from Bullet.
	 * @param corner## Coordinates of the patch corners.
	 * @param resx Horizontal resolution in triangles.
	 * @param resy Vertical resolution in triangles.
	 * @param fixeds Fixed corners (values: 0 | 1 | 2 | 4 | 8)
	 */
	Cloth(btSoftBodyWorldInfo& worldInfo,
		const btVector3& corner00,
		const btVector3& corner10,
		const btVector3& corner01,
		const btVector3& corner11,
		int resx,
		int resy,
		int fixeds);
	~Cloth(void);

	void render(glm::mat4 parentTransform);
	bool addToWorld(btDynamicsWorld * world);

private:
	static GLuint vbo;
	static GLint posAttrib;
	static GLint colAttrib;
	static GLuint sm_shaderProgram;
	
	btSoftBody * softBody; //< Bullet's internal representation of the cloth
	
	// Fragment shader variables
	static GLint uniPVM;

	glm::mat4 model; //< local transformation matrix
};

