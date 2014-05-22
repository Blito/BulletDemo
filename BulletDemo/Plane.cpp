#include "Plane.h"

#include <SDL2/SDL_opengl.h>
#include <bullet\BulletCollision\CollisionShapes\btStaticPlaneShape.h>
#include <bullet\LinearMath\btDefaultMotionState.h>

Plane::Plane(int y, unsigned width, unsigned depth) : y(y), width(width), depth(depth) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0,0,0));
	btStaticPlaneShape * plane = new btStaticPlaneShape(btVector3(0,1,0), y);
	btMotionState * motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0, motion, plane);
	rigidBody = new btRigidBody(info);
}

void Plane::render() {
	glPushMatrix();

	glTranslatef(0,y,0);
	glScalef(width, 0, depth);

	glBegin(GL_QUADS);

	glColor3f(0.7, 0.7, 0.7);
	glVertex3f(-1.0f, y, 1.0f);
	glVertex3f(-1.0f, y, -1.0f);
	glVertex3f(1.0f, y, -1.0f);
	glVertex3f(1.0f, y, 1.0f);

	glEnd();

	glPopMatrix();
}

bool Plane::addToWorld(btDynamicsWorld * world) {
	world->addRigidBody(rigidBody);
	return true;
}