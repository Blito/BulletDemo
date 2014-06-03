#include "Box.h"

#include <bullet\LinearMath\btDefaultMotionState.h>
#include <bullet\BulletCollision\CollisionShapes\btBoxShape.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader
const GLchar* Box::vertexSource = 
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
const GLchar* Box::fragmentSource = 
	"#version 150 core\n"
	"in vec3 Color;"
	"out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(Color, 1.0);"
    "}";

GLint Box::uniPVM = 0;
GLuint Box::vbo = 0;
GLint Box::posAttrib = 0;
GLint Box::colAttrib = 0;
GLuint Box::shaderProgram = 0;

bool Box::load() {
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
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 2
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 3
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 3
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 4
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1

        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 5
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 6
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 7
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 7
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 8
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 5

        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 8
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 4
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 5
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 8

         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 7
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 3
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 2
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 2
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 6
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 7

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 1
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 2
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 6
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 6
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 5
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 1

        -0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // 4
         0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // 3
         0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // 7
         0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // 7
        -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, // 8
        -0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f  // 4
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
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
	// in vec2 position;
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

Box::Box(float width, float height, float depth, float x, float y, float z, float mass) 
	: width(width), height(height), depth(depth) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x,y,z));
	btBoxShape * cube = new btBoxShape(btVector3(width/2, height/2, depth/2));
	btVector3 inertia(0,0,0);
	if (mass != 0.0) {
		cube->calculateLocalInertia(mass, inertia);
	}

	btMotionState * motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cube, inertia);
	rigidBody = new btRigidBody(info);
}


Box::~Box(void)
{
}

void Box::render(glm::mat4 parentTransform) {
	
	float mat[16];
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	t.getOpenGLMatrix(mat);

	glm::mat4 model = glm::make_mat4(mat);

	glm::mat4 pvm = parentTransform * model * glm::scale(glm::vec3(width, height, depth));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glUniformMatrix4fv(uniPVM, 1, GL_FALSE, glm::value_ptr(pvm));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

bool Box::addToWorld(btDynamicsWorld * world) {
	world->addRigidBody(rigidBody);
	return true;
}