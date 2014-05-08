#pragma once
#include "renderedobject.h"
#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>
#include <bullet\BulletSoftBody\btSoftBodyHelpers.h>

class Cloth :
	public RenderedObject
{
public:
	Cloth(btSoftBodyWorldInfo& worldInfo,
		const btVector3& corner00,
		const btVector3& corner10,
		const btVector3& corner01,
		const btVector3& corner11,
		int resx,
		int resy,
		int fixeds);
	~Cloth(void);

	void render();
	bool addToWorld(btDynamicsWorld * world);

private:
	btSoftBody * softBody;
};

