#include "Plane.h"

#include <bullet\BulletCollision\CollisionShapes\btStaticPlaneShape.h>
#include <bullet\LinearMath\btDefaultMotionState.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader
const GLchar* Plane::vertexSource = 
	"#version 150 core\n"
	"in vec3 position;"
	"in vec3 color;"
	"out vec3 Color;"
	"uniform mat4 pvm;"
	"void main() {"
	"	Color = color;"
	"   gl_Position = pvm * vec4(position, 1.0);"
	"}";

// Fragment Shader
const GLchar* Plane::fragmentSource = 
	"#version 150 core\n"
	"in vec3 Color;"
	"out vec4 outColor;"
	"void main() {"
	"   outColor = vec4(Color, 1.0);"
	"}";

GLint Plane::uniPVM = 0;
GLuint Plane::vbo = 0;
GLint Plane::posAttrib = 0;
GLint Plane::colAttrib = 0;
GLuint Plane::shaderProgram = 0;

bool Plane::load() {
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

	/*GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(elements), elements, GL_STATIC_DRAW);*/

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
	// in vec3 position;
    posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	// in vec3 color;
	colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	uniPVM = glGetUniformLocation(shaderProgram, "pvm");
	
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

void Plane::render(glm::mat4 parentTransform) {

	glm::mat4 model;
	model = glm::scale(glm::vec3(width, 1.0, depth)) 
		  * glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) 
		  * glm::translate(glm::vec3(0.0f, 0.0f, y));

	glm::mat4 pvm = parentTransform * model;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glUniformMatrix4fv(uniPVM, 1, GL_FALSE, glm::value_ptr(pvm));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

bool Plane::addToWorld(btDynamicsWorld * world) {
	world->addRigidBody(rigidBody);
	return true;
}