#include "RigidObject.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include "ShaderMgr.h"

GLuint RigidObject::sm_shaderProgram = 0;
GLint RigidObject::uniPVM = 0;
GLint RigidObject::uniModel = 0;

void RigidObject::load() {
	ShaderMgr * shaderMgr = ShaderMgr::GetSingletonPtr();

	sm_shaderProgram = shaderMgr->createProgram("../BulletDemo/dirlightdiffambpix.vert", "../BulletDemo/dirlightdiffambpix.frag");
}

RigidObject::RigidObject(const std::string& filename)
{

	m_renderMgr = RenderMgr::GetSingletonPtr();

	if (!import3DFromFile(filename)) {
		return;
	}

	genVAOsAndUniformBuffer(object);

	uniPVM = glGetUniformLocation(sm_shaderProgram, "pvm");
	uniModel = glGetUniformLocation(sm_shaderProgram, "model");

}


RigidObject::~RigidObject(void)
{
}

void RigidObject::render(const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & preMult) {

	glUseProgram(sm_shaderProgram);


	
}

bool RigidObject::addToWorld(btDynamicsWorld * world) {
	return false;
}

bool RigidObject::import3DFromFile( const std::string& filename)
{
	Assimp::Importer importer;

	//check if file exists
	std::ifstream fin(filename.c_str());
	if(!fin.fail()) {
		fin.close();
	} else {
		printf("Couldn't open file: %s\n", filename.c_str());
		printf("%s\n", importer.GetErrorString());
		return false;
	}

	object = importer.ReadFile( filename, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if(!object)
	{
		printf("%s\n", importer.GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	printf("Import of object %s succeeded.",filename.c_str());
	
	aiVector3D object_min, object_max, object_center;
	getBoundingBox(*object,&object_min, &object_max);
	float tmp;
	tmp = object_max.x-object_min.x;
	tmp = object_max.y - object_min.y > tmp?object_max.y - object_min.y:tmp;
	tmp = object_max.z - object_min.z > tmp?object_max.z - object_min.z:tmp;
	scaleFactor = 1.f / tmp;

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

void RigidObject::getBoundingBox (const aiScene & object, aiVector3D* min, aiVector3D* max)
{
	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	getBoundingBoxForNode(object,object.mRootNode,min,max);
}

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

void RigidObject::getBoundingBoxForNode (const aiScene & object, const aiNode* nd, 
	aiVector3D* min, 
	aiVector3D* max)
{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = object.mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		getBoundingBoxForNode(object,nd->mChildren[n],min,max);
	}
}

void RigidObject::genVAOsAndUniformBuffer(const aiScene *sc) {

	ShaderMgr * shaderMgr = ShaderMgr::GetSingletonPtr();

	// Vertex Attribute Locations
	GLuint vertexLoc = shaderMgr->getAttribLocation(ShaderMgr::c_verticesAttr);
	GLuint normalLoc = shaderMgr->getAttribLocation(ShaderMgr::c_normalAttr);
	GLuint texCoordLoc = shaderMgr->getAttribLocation(ShaderMgr::c_texCoordAttr);

	// Uniform Bindings Points
	GLuint matricesUniLoc = 1, materialUniLoc = 2;

	struct MyMesh aMesh;
	struct MyMaterial aMat; 
	GLuint buffer;
	
	// For each mesh
	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{
		const aiMesh* mesh = sc->mMeshes[n];

		// create array with faces
		// have to convert from Assimp format to array
		unsigned int *faceArray;
		faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
		unsigned int faceIndex = 0;

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];

			memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
			faceIndex += 3;
		}
		aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

		// generate Vertex Array for mesh
		glGenVertexArrays(1,&(aMesh.vao));
		glBindVertexArray(aMesh.vao);

		// buffer for faces
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

		// buffer for vertex positions
		if (mesh->HasPositions()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(vertexLoc);
			glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
		}

		// buffer for vertex normals
		if (mesh->HasNormals()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
			glEnableVertexAttribArray(normalLoc);
			glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
		}

		// buffer for vertex texture coordinates
		if (mesh->HasTextureCoords(0)) {
			float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

				texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
				texCoords[k*2+1] = mesh->mTextureCoords[0][k].y; 
				
			}
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(texCoordLoc);
			glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
		}

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
		// create material uniform buffer
		aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];
			
		aiString texPath;	//contains filename of texture
		if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
				//bind texture
				unsigned int texId = textureIdMap[texPath.data];
				aMesh.texIndex = texId;
				aMat.texCount = 1;
			}
		else
			aMat.texCount = 0;

		float c[4];
		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
		aiColor4D diffuse;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			color4_to_float4(&diffuse, c);
		memcpy(aMat.diffuse, c, sizeof(c));

		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
		aiColor4D ambient;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			color4_to_float4(&ambient, c);
		memcpy(aMat.ambient, c, sizeof(c));

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D specular;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			color4_to_float4(&specular, c);
		memcpy(aMat.specular, c, sizeof(c));

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D emission;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
			color4_to_float4(&emission, c);
		memcpy(aMat.emissive, c, sizeof(c));

		float shininess = 0.0;
		unsigned int max;
		aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
		aMat.shininess = shininess;

		glGenBuffers(1,&(aMesh.uniformBlockIndex));
		glBindBuffer(GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);

		myMeshes.push_back(aMesh);
	}
}

void RigidObject::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void RigidObject::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void RigidObject::recursiveRender(const aiScene *sc, const aiNode* nd) {

	// Get node transformation matrix
	aiMatrix4x4 m = nd->mTransformation;
	// OpenGL matrices are column major
	m.Transpose();

	// save model matrix and apply node transformation
	glm::mat4 model;
	copyAiMatrixToGLM(&m, model);
	m_renderMgr->pushMatrix(model);
		
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	// draw all meshes assigned to this node
	for (unsigned int n=0; n < nd->mNumMeshes; ++n){
		// bind material uniform
		//glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial));	
		// bind texture
		glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
		// bind VAO
		glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
		// draw
		glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0);

	}

	// draw all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n){
		recursiveRender(sc, nd->mChildren[n]);
	}

	m_renderMgr->popMatrix();
}

inline void RigidObject::copyAiMatrixToGLM(const aiMatrix4x4 *from, glm::mat4 &to)
{
        to[0][0] = (GLfloat)from->a1; to[1][0] = (GLfloat)from->a2;
        to[2][0] = (GLfloat)from->a3; to[3][0] = (GLfloat)from->a4;
        to[0][1] = (GLfloat)from->b1; to[1][1] = (GLfloat)from->b2;
        to[2][1] = (GLfloat)from->b3; to[3][1] = (GLfloat)from->b4;
        to[0][2] = (GLfloat)from->c1; to[1][2] = (GLfloat)from->c2;
        to[2][2] = (GLfloat)from->c3; to[3][2] = (GLfloat)from->c4;
        to[0][3] = (GLfloat)from->d1; to[1][3] = (GLfloat)from->d2;
        to[2][3] = (GLfloat)from->d3; to[3][3] = (GLfloat)from->d4;
}