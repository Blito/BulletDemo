#pragma once
#include "renderedobject.h"
#include <GL\glew.h>
#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>   // (AssImp) Output data structure

#include "RenderMgr.h"

class RigidObject :
	public RenderedObject
{
public:
	static void load();
	RigidObject (const std::string& filename);
	~RigidObject (void);
	void render (const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & preMult);
	bool addToWorld (btDynamicsWorld * world);

protected:

	static GLuint sm_shaderProgram;

	RenderMgr * m_renderMgr;

	// Information to render each assimp node
	struct MyMesh{

		GLuint vao;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		int numFaces;
	};

	// This is for a shader uniform block
	struct MyMaterial{

		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];
		float shininess;
		int texCount;
	};

	std::vector<struct MyMesh> myMeshes;
	const aiScene * object;
	int numVerts;
	float scaleFactor;

	// map image filenames to textureIds
	// pointer to texture Array
	std::map<std::string, GLuint> textureIdMap;	

private:
	bool import3DFromFile (const std::string & filename);
	void getBoundingBox (aiVector3D* min, aiVector3D* max);
	void getBoundingBoxForNode (const aiNode* nd, aiVector3D* min, aiVector3D* max);
	void genVAOsAndUniformBuffer();
	void recursiveRender(const aiNode* nd, const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & preMult);

	// Fragment shader variables
	static GLint uniPVM, uniModel;

	Assimp::Importer importer;

	// TODO: move to utils
	void set_float4(float f[4], float a, float b, float c, float d);
	void color4_to_float4(const aiColor4D *c, float f[4]);
	inline void copyAiMatrixToGLM(const aiMatrix4x4 *from, glm::mat4 &to);
};

