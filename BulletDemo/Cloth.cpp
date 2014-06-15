#include "Cloth.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "ShaderMgr.h"

GLint Cloth::uniPVM = 0;
GLuint Cloth::vbo = 0;
GLint Cloth::posAttrib = 0;
GLint Cloth::colAttrib = 0;
GLuint Cloth::sm_shaderProgram = 0;

bool Cloth::load(GLuint shaderProgram) {

	sm_shaderProgram = shaderProgram;
	ShaderMgr * shaderMgr = ShaderMgr::GetSingletonPtr();

	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &vbo);
	
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/  

    // Specify the layout of the vertex data
	// in vec3 position;
    posAttrib = shaderMgr->getAttribLocation(ShaderMgr::c_verticesAttr);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	// in vec3 color;
	colAttrib = shaderMgr->getAttribLocation(ShaderMgr::c_colorAttr);
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	uniPVM = glGetUniformLocation(shaderProgram, "pvm");
	
	return true;
}

Cloth::Cloth(btSoftBodyWorldInfo& worldInfo,
		const btVector3& corner00,
		const btVector3& corner10,
		const btVector3& corner01,
		const btVector3& corner11,
		int resx,
		int resy,
		int fixeds)
{
	softBody = btSoftBodyHelpers::CreatePatch(worldInfo, corner00, corner10, corner01, corner11, resx, resy, fixeds, true);
	softBody->setTotalMass(1.0);
	softBody->m_cfg.viterations = 30;
	softBody->m_cfg.piterations = 30;
}


Cloth::~Cloth(void)
{
}

void Cloth::render(glm::mat4 parentTransform) {
	
	ShaderMgr::GetSingleton().useShader(sm_shaderProgram);

	int vertCount = softBody->m_faces.size()*3*8;
	GLfloat * vertices = new GLfloat[vertCount];
	// TODO: delete vertices;

	// For each face in the cloth
	for (int i = 0; i < softBody->m_faces.size(); i++) {
		// For each vertex in the face
		for (int j = 0; j < 3; j++) {
			vertices[(i*3+j)*8] = softBody->m_faces[i].m_n[j]->m_x.x();   // x 
			vertices[(i*3+j)*8+1] = softBody->m_faces[i].m_n[j]->m_x.y(); // y 
			vertices[(i*3+j)*8+2] = softBody->m_faces[i].m_n[j]->m_x.z(); // z 
			vertices[(i*3+j)*8+3] = 1.0f;								  // r 
			vertices[(i*3+j)*8+4] = 0.0f;								  // g 
			vertices[(i*3+j)*8+5] = 1.0f;								  // b 
			vertices[(i*3+j)*8+6] = 0.0f;								  // u 
			vertices[(i*3+j)*8+7] = 0.0f;								  // v 
		}
	}

	glm::mat4 pvm = parentTransform;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertCount, &(*vertices), GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glUniformMatrix4fv(uniPVM, 1, GL_FALSE, glm::value_ptr(pvm));

	glDrawArrays(GL_TRIANGLES, 0, vertCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Cloth::addToWorld(btDynamicsWorld * world) {
	if (world->getWorldType() == btDynamicsWorldType::BT_SOFT_RIGID_DYNAMICS_WORLD) {
		((btSoftRigidDynamicsWorld*)world)->addSoftBody(softBody);
		return true;
	}
	return false;
}