#include "RigidObject.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include <SDL2/SDL_opengl.h>

RigidObject::RigidObject(std::string& filename)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	
	const aiScene * object = importer.ReadFile( filename, 
		aiProcess_CalcTangentSpace       | 
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType);

	if (!object) {
		// TODO: error handling
		return;
	}
}


RigidObject::~RigidObject(void)
{
}

void RigidObject::render(glm::mat4 parentTransform) {
	
}

bool RigidObject::addToWorld(btDynamicsWorld * world) {
	return false;
}