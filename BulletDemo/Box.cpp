#include "Box.h"
#include <bullet\LinearMath\btDefaultMotionState.h>
#include <bullet\BulletCollision\CollisionShapes\btBoxShape.h>
#include <glm/gtc/matrix_transform.hpp>
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

bool Box::load() {
	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

	// Create an Element Buffer Object
	GLuint ebo;
	glGenBuffers(1, &ebo);

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
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
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	// in vec3 color;
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	uniPVM = glGetUniformLocation(shaderProgram, "pvm");
	
	return true;
}

Box::Box(float width, float height, float depth, float x, float y, float z, float mass) {
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

	glm::mat4 model;

	float mat[16];
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	t.getOpenGLMatrix(mat);

	model = glm::make_mat4(mat);;

	glm::mat4 pvm = parentTransform * model;

	glUniformMatrix4fv(uniPVM, 1, GL_FALSE, glm::value_ptr(pvm));

	glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	/*btVector3 extent = ((btBoxShape*)rigidBody->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
        glMultMatrixf(mat);     //translation,rotation
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),extent.z());
                glVertex3f(-extent.x(),extent.y(),extent.z());         
        glEnd();
		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(extent.x(),extent.y(),-extent.z());
                glVertex3f(extent.x(),-extent.y(),-extent.z());
                glVertex3f(extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),extent.z());          
        glEnd();
		glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),extent.z());
                glVertex3f(-extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),extent.z());          
        glEnd();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),-extent.z());
                glVertex3f(extent.x(),-extent.y(),-extent.z());
                glVertex3f(extent.x(),extent.y(),-extent.z());         
        glEnd();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),extent.y(),-extent.z());
                glVertex3f(-extent.x(),extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),extent.z());
                glVertex3f(extent.x(),extent.y(),-extent.z());         
        glEnd();
		glColor4f(0.5f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
                glVertex3f(-extent.x(),-extent.y(),-extent.z());
                glVertex3f(-extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),-extent.y(),extent.z());
                glVertex3f(extent.x(),-extent.y(),-extent.z());        
        glEnd();               
    glPopMatrix();*/
}

bool Box::addToWorld(btDynamicsWorld * world) {
	world->addRigidBody(rigidBody);
	return true;
}