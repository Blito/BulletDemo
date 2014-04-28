#include "Box.h"

#include <SDL2/SDL_opengl.h>
#include <bullet\LinearMath\btDefaultMotionState.h>
#include <bullet\BulletCollision\CollisionShapes\btBoxShape.h>

Box::Box(float width, float height, float depth, float x, float y, float z, float mass) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x,y,z));
	btBoxShape * cube = new btBoxShape(btVector3(width/2, height/2, depth/2));
	btVector3 inertia(0,0,0);
	if (mass != 0.0) {
		cube->calculateLocalInertia(mass, inertia);
	}

	btMotionState * motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cube, inertia);
	rigidBody = new btRigidBody(info);
}


Box::~Box(void)
{
}

void Box::render() {
	btVector3 extent = ((btBoxShape*)rigidBody->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
        glMultMatrixf(mat);     //translation,rotation
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),extent.z());
                glVertex3f(-extent.x(),extent.y(),extent.z());         
        glEnd();
		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(extent.x(),extent.y(),-extent.z());
                glVertex3f(extent.x(),-extent.y(),-extent.z());
                glVertex3f(extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),extent.z());          
        glEnd();
		glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),extent.z());
                glVertex3f(-extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),extent.z());          
        glEnd();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),-extent.z());
                glVertex3f(extent.x(),-extent.y(),-extent.z());
                glVertex3f(extent.x(),extent.y(),-extent.z());         
        glEnd();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),-extent.z());
                glVertex3f(-extent.x(),extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),-extent.z());         
        glEnd();
		glColor4f(0.5f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),-extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),-extent.y(),-extent.z());        
        glEnd();               
    glPopMatrix();
}