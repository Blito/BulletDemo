#include "PhysicsMgr.h"

// Bullet includes
#include <bullet\LinearMath\btTransform.h>
#include <bullet\BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h>
#include <bullet\BulletSoftBody\btDefaultSoftBodySolver.h>
#include <bullet\BulletCollision\CollisionShapes\btBoxShape.h>
#include <bullet\BulletCollision\BroadphaseCollision\btDispatcher.h>
#include <bullet\BulletCollision\BroadphaseCollision\btDbvtBroadphase.h>
#include <bullet\BulletDynamics\ConstraintSolver\btSequentialImpulseConstraintSolver.h>

namespace LittleLab {

	namespace Physics {

		PhysicsMgr::PhysicsMgr()
		{
			// Physics init
			collisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfig);
			broadphase = new btDbvtBroadphase();
			solver = new btSequentialImpulseConstraintSolver();
			softbodySolver = new btDefaultSoftBodySolver();
			world = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfig, softbodySolver);
			world->setGravity(btVector3(0, -10.0f, 0));
		}


		PhysicsMgr::~PhysicsMgr()
		{
			// Physics
			delete world;
			delete solver;
			delete broadphase;
			delete dispatcher;
			delete collisionConfig;
		}

	}

}