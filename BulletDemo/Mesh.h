#pragma once

#define GL_GLEXT_PROTOTYPES
#include "SDL2\SDL_opengl.h"

#include "RenderedObject.h"

#include "assimp\scene.h"
#include "assimp\mesh.h"

#include <vector>

class Mesh : public RenderedObject
{
public :
	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;

		MeshEntry(aiMesh *mesh);
		~MeshEntry();

		void load(aiMesh *mesh);
		void render();
	};

	std::vector<MeshEntry*> meshEntries;

public:
	Mesh(const char *filename);
	~Mesh(void);

	void render();
	bool addToWorld(btDynamicsWorld * world) { return false; }
};