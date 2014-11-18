#pragma once

#include "singleton.h"

#include <bullet\BulletSoftBody\btSoftRigidDynamicsWorld.h>

namespace LittleLab {

	namespace Physics {

		typedef btSoftRigidDynamicsWorld World;

		class PhysicsMgr : public Utils::Singleton<PhysicsMgr>
		{
		public:
			PhysicsMgr();
			virtual ~PhysicsMgr();

			inline World * getWorld() const { return world; };

		protected:
			// Physics
			btSoftRigidDynamicsWorld * world;
			btDispatcher * dispatcher;
			btCollisionConfiguration * collisionConfig;
			btBroadphaseInterface * broadphase;
			btConstraintSolver * solver;
			btSoftBodySolver * softbodySolver;
		};

	}

}