#pragma once
#include "Scene.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GLU.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <math.h>
#include <bullet\BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <bullet\BulletCollision\BroadphaseCollision\btDispatcher.h>
#include <bullet\BulletCollision\BroadphaseCollision\btDbvtBroadphase.h>
#include <bullet\BulletDynamics\ConstraintSolver\btSequentialImpulseConstraintSolver.h>
#include <bullet\BulletCollision\CollisionDispatch\btCollisionConfiguration.h>
#include <bullet\BulletDynamics\Dynamics\btRigidBody.h>

class SceneExample :
	public Scene
{
public:
	SceneExample(void);
	~SceneExample(void);
	void update(Uint32 elapsedTimeInMillis);
	void render();
	bool isQuit() { return quit; }
	SDL_Texture * getText() const { return text; }

private:
	void createCube(float size, float x = 0, float y = 0, float z = 0, float mass = 0);
	void renderCube(float size, float x = 0, float y = 0, float z = 0);
	void renderPlane(float y = 0);
	void createText(const std::string &message, SDL_Color color);

	// Camera
	float angleH, angleV;
	float xCam, yCam, zCam;
	float lx, ly, lz;

	// Movemnet
	float mov_speed, rot_speed, mouse_sensitivity;

	// Physics
	btDynamicsWorld * world;
	btDispatcher * dispatcher;
	btCollisionConfiguration * collisionConfig;
	btBroadPhaseInterface * broadphase;
	btConstraintSolver * solver;
	std::vector<btRigidBody*> bodies;

	bool quit;
	TTF_Font *font;
	SDL_Texture * text;
};

