#include "Box.h"
#include <bullet\LinearMath\btDefaultMotionState.h>
#include <bullet\BulletCollision\CollisionShapes\btBoxShape.h>

const GLchar* Box::vertexSource = 
	"#version 150 core\n"
    "in vec2 position;"
    "void main() {"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";

const GLchar* Box::fragmentSource = 
	"#version 150 core\n"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";

bool Box::load() {
	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

void Box::render() {
	// Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
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