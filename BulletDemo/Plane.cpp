#include "Plane.h"

#include <bullet\BulletCollision\CollisionShapes\btStaticPlaneShape.h>
#include <bullet\LinearMath\btDefaultMotionState.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderMgr.h"

GLint Plane::uniPVM = 0;
GLint Plane::uniModel = 0;
GLuint Plane::vbo = 0;
GLint Plane::posAttrib = 0;
GLint Plane::colAttrib = 0;
GLuint Plane::sm_shaderProgram = 0;

bool Plane::load(GLuint shaderProgram) {

	sm_shaderProgram = shaderProgram;

	ShaderMgr * shaderMgr = ShaderMgr::GetSingletonPtr();

	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &vbo);

	// Create an Element Buffer Object
	GLuint ebo;
	glGenBuffers(1, &ebo);

    GLfloat vertices[] = {
		-1.0f,  1.0f, 0.0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
		 1.0f,  1.0f, 0.0, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top-right
		 1.0f, -1.0f, 0.0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right

		 1.0f, -1.0f, 0.0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
		-1.0f, -1.0f, 0.0, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom-left
		-1.0f,  1.0f, 0.0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f  // Top-left
    };


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
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
	uniModel = glGetUniformLocation(shaderProgram, "model");
	
	return true;
}

Plane::Plane(int y, unsigned width, unsigned depth) : y(y), width(width), depth(depth) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0,0,0));
	btStaticPlaneShape * plane = new btStaticPlaneShape(btVector3(0,1,0), y);
	btMotionState * motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0, motion, plane);
	rigidBody = new btRigidBody(info);
}

void Plane::render(const glm::mat4 & proj, const glm::mat4 & view, const glm::mat4 & preMult) {
	
	ShaderMgr::GetSingleton().useShader(sm_shaderProgram);

	glm::mat4 model;
	model = glm::scale(glm::vec3(width, 1.0, depth)) 
		  * glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) 
		  * glm::translate(glm::vec3(0.0f, 0.0f, y));

	glm::mat4 pvm = preMult * model;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glUniformMatrix4fv(uniPVM, 1, GL_FALSE, glm::value_ptr(pvm));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

bool Plane::addToWorld(btDynamicsWorld * world) {
	world->addRigidBody(rigidBody);
	return true;
}